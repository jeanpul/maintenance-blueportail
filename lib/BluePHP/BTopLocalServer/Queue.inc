<?php

class Queue
{
  private $params = null;
  private $idx = array();
  private $name = null;
  private $alarm = array( "inAlarm" => 0,
			  "inRZ1" => 0,
			  "inRZ2" => 0 );

  private $stats = null;

  private $data = array( "time" => array(), 
			 "wt" => array(),
			 "numbering" => array(),
			 "tf" => array(),
			 "flow" => array(),
			 "rwt" => array() );

  function __construct($idx, $name, $params) 
  {
    $this->idx = $idx;
    $this->name = $name;
    $this->params = $params;
    $this->reset();    
  }

  public function getName()
  {
    return $this->name;
  }

  public function reset()
  {
    $this->stats = array( "NbValues" => 0,
			  "TotalWT" => 0,
			  "MaxWT" => 0,
			  "MaxN" => 0,
			  "MeanWT" => 0,
			  "TotalAlarm" => 0,
			  "%LA" => 0,
			  "TotalAct" => 0,
			  "%ACT" => 0,
			  "RZ1" => 0,
			  "RZ2" => 0,
			  "MaxRZ" => 0 );
  }

  public function update($cols)
  {
    if(!isset($cols[$this->idx["WT"]]) or
       !isset($cols[$this->idx["N"]]) or
       !isset($cols[$this->idx["WTF"]]))
      {
	return false;
      }

    $this->stats["NbValues"] += 1;

    $wt = (int) $cols[$this->idx["WT"]];
    $this->stats["TotalWT"] += $wt;
    if($wt > $this->stats["MaxWT"])
      {
	$this->stats["MaxWT"] = $wt;
      }
    $num = (int) $cols[$this->idx["N"]];
    if($num > 0)
      {
	$this->stats["TotalAct"] += 1;
      }        
    if($num > $this->stats["MaxN"])
      {
	$this->stats["MaxN"] = $num;
      }

    $flow = (int) $cols[$this->idx["WTF"]];

    $time = substr($cols[0], -8, 5); // HH:mm
    $this->data["time"][] = $time;
    $this->data["wt"][] = $wt;
    $rwt = $cols[$this->idx["RWT"]];
    $this->data["rwt"][] = empty($rwt) ? null : (int) $rwt;
    $this->data["numbering"][] = $num;

    $this->data["flow"][] = $flow;
		      

    if($wt > $this->params["alarms"]["WT"])
      {
	$this->stats["TotalAlarm"] += 1;
	$this->alarm["inAlarm"] += 1;
      }
    else
      {
	$this->alarm["inAlarm"] = 0;
      }
    if($this->alarm["inAlarm"] > $this->stats["MaxRZ"])
      {
	$this->stats["MaxRZ"] = $this->alarm["inAlarm"];
      }
    if(!$this->alarm["inRZ1"])
      {
	$this->alarm["inRZ1"] = $this->alarm["inAlarm"] > $this->params["alarms"]["RZ1"] ? 1 : 0;
      }
    else
      {
	$this->alarm["inRZ1"] = 0;
      }
    if(!$this->alarm["inRZ2"])
      {
	$this->alarm["inRZ2"] = $this->alarm["inAlarm"] > $this->params["alarms"]["RZ2"] ? 1 : 0;
      }
    else
      {
	$this->alarm["inRZ2"] = 0;
      }
    $this->stats["RZ1"] += $this->alarm["inRZ1"];
    $this->stats["RZ2"] += $this->alarm["inRZ2"];

    $this->data["tf"][] = $this->alarm["inAlarm"];
  }

  public function updateGlobal()
  {
    if($this->getStat("NbValues"))
      {
	if($this->getStat("TotalAct"))
	  {
	    $this->setStat("MeanWT", $this->getStat("TotalWT") / $this->getStat("TotalAct"));
	    $this->setStat("%LA", (1 - $this->getStat("TotalAlarm") / $this->getStat("TotalAct")) * 100.);
	    $this->setStat("%ACT", $this->getStat("TotalAct") / $this->getStat("NbValues") * 100.);
	  }
	else
	  {
	    $this->setStat("MeanWT", 0.);
	    $this->setStat("%LA", 100.);
	    $this->setStat("%ACT", 0.);
	  }
      }
  }

  public function setStat($name, $value)
  { 
    $this->stats[$name] = $value;
  }
    
  public function getStat($name)
  {
    return $this->stats[$name];
  }

  public function getMaxValue()
  {
    return $this->getStat("MaxWT");
  }

  public function getThreshold()
  {
    return $this->params["alarms"]["WT"];
  }

  public function getWTValues()
  {
    return $this->data["wt"];
  }

  public function getRWTValues()
  {
    return $this->data["rwt"];
  }

  public function getNValues()
  {
    return $this->data["numbering"];
  }

  public function getTimeValues()
  {
    return $this->data["time"];
  }

  public function getTfValues()
  {
    return $this->data["tf"];
  }

  public function getFlow()
  {
    return $this->data["flow"];
  }
}

?>
