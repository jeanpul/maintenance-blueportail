<?php

include_once("Queue.inc");

class QueueProcess
{
  private $params = array();

  private $queueIdx = array();
  private $queueName = array();

  private $data = null;
  private $stats = null;

  function __construct($queueName, $params)
  {    
    $this->queueName = $queueName;
    $this->params = $params;
    $this->reset();
  }

  public function reset()
  {
    $this->data = array();
    $this->stats = array( "NbValues" => 0,
			  "MeanWT" => 0,
			  "MaxWT" => 0,
			  "MaxN" => 0,
			  "%LA" => 0,
			  "MeanAct" => 0,
			  "%Act" => 0,
			  "RZ1" => 0,
			  "RZ2" => 0,
			  "MaxRZ" => 0 );
    $this->nbValues = 0;
    $this->queueIdx = array();
  }

  public function process($D)
  {
    $res = $this->create_queue_idx(array_map("trim", explode(",", $D[0])));
    if($res)
      {
	for($i = 1; $i < count($D); $i++)
	  {
	    $cols = array_map("trim", explode(",", $D[$i]));
	    $hour = (int) substr( $cols[0], -8, 2); // HH
	    if($hour >= $this->params["hourLimits"]["min"] and
	       $hour <= $this->params["hourLimits"]["max"])
	      {
		$this->data[] = $cols;
		$this->stats["NbValues"] += 1;
		$this->update_queue_stats($cols);
	      }
	  }
	$this->update_global_stats();
      }
    return $res;
  }

  private function create_queue_idx($cols)
  {
    foreach($this->queueName as $k => $v)
      {
	$idx = array(); 
	for($i = 0; $i < count($cols); $i++)
	  {
	    if($cols[$i] == "WT_" . $v)
	      {
		$idx["WT"] = $i;
	      }
	    else if($cols[$i] == "WTF_" . $v)
	      {
		$idx["WTF"] = $i;
	      }
	    else if($cols[$i] == "N_" . $v)
	      {
		$idx["N"] = $i;
	      }
	    else if($cols[$i] == "RWT_" . $v)
	      {
		$idx["RWT"] = $i;
	      }
	  }	

	if(isset($idx["WT"]) and 
	   isset($idx["WTF"]) and 
	   isset($idx["N"]) and
	   isset($idx["RWT"]))
	  {
	    $this->queueIdx[$v] = new Queue($idx, $v, $this->params);
	  }
      }
    return count($this->queueIdx);
  }

  private function update_queue_stats($cols)
  {
    foreach($this->queueIdx as $k => $v)
      {
	$v->update($cols);
      }
  }

  private function update_global_stats()
  {
    if(count($this->queueIdx))
      {
	$totalWT = 0;
	$totalAct = 0;
	$totalLA = 0;
	foreach($this->queueIdx as $k => $v)
	  {
	    $v->updateGlobal();
	    $totalWT += $v->getStat("MeanWT");
	    $maxWT = $v->getStat("MaxWT");
	    if($maxWT > $this->getStat("MaxWT"))
	      {
		$this->setStat("MaxWT", $maxWT);
	      }
	    $maxN = $v->getStat("MaxN");
	    if($maxN > $this->getStat("MaxN"))
	      {
		$this->setStat("MaxN", $maxN);
	      }
	    $totalAct += $v->getStat("TotalAct");	    
	    $this->stats["RZ1"] += $v->getStat("RZ1");
	    $this->stats["RZ2"] += $v->getStat("RZ2");
	    $this->stats["MaxRZ"] = max($this->stats["MaxRZ"], $v->getStat("MaxRZ"));
	    $totalLA += $v->getStat("%LA");
	  }
	$this->setStat("MeanWT", $totalWT / count($this->queueIdx));
	$this->setStat("MeanAct", $totalAct / count($this->queueIdx));
	$this->setStat("%LA", $totalLA / count($this->queueIdx));
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

  /**
   * Common methods with CashProcess
   * will/must be inherited from a common
   * base class
   */
  public function getTotalMeanValue()
  {
    return $this->getStat("MeanWT");
  }

  public function getMaxValue()
  {
    return $this->getStat("MaxWT");
  }

  public function getMaxN()
  {
    return $this->getStat("MaxN");
  }

  public function getThresholdRate()
  {
    return round($this->getStat("%LA"), 0);
  }
  
  public function getTotalMeanActivity()
  {
    return round($this->getStat("MeanAct"), 0);
  }

  public function getActivityRate()
  {
    return round($this->getStat("MeanAct") / $this->getStat("NbValues") * 100., 0);
  }

  public function getNbAlarmDM()
  {
    return $this->getStat("RZ1");
  }

  public function getNbAlarmDR()
  {
    return $this->getStat("RZ2");
  }

  public function getMaxTf()
  {
    return $this->getStat("MaxRZ");
  }

  public function getQueuesData()
  {
    $data = array();
    foreach($this->queueIdx as $k => $v)
      {
	$data[] = array( "id" => $v->getName(),
			 "value" => $v->getStat("MeanWT"),
			 "MaxN" => $v->getStat("MaxN"),
			 "MaxWT" => $v->getStat("MaxWT"),
			 "activity" => $v->getStat("TotalAct"),
			 "thresholdRate" => $v->getStat("%LA"),
			 "arate" => $v->getStat("%ACT"),
			 "alarmDM" => $v->getStat("RZ1"),
			 "alarmDR" => $v->getStat("RZ2"),
			 "maxTf" => $v->getStat("MaxRZ") );
      }    
    return $data;
  }

  public function getQueue($name)
  {
    return $this->queueIdx[$name];
  }

  public function getThreshold()
  {
    return $this->params["alarms"]["WT"];
  }

  
}

?>
