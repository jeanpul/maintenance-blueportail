<?php

class CashSet 
{
  var $site = 0;
  var $cashes = array();
  var $data = array();
  var $limitFull = 2;
  var $tf = 0;
  var $maxTf = 5; 
  var $maxTf2 = 15; // hyper

  function CashSet($cashes)
  {
    $this->cashes = $cashes;
    $this->init_data();
  }

  function init_data()
  {
    $this->data = array( "date" => false, 
			 "open" => 0, 
			 "close" => 0, 
			 "full" => 0, 
			 "alarmDM" => 0,
			 "alarmDR" => 0,
			 "total" => 0,
			 "MeanValue" => 0,
			 "MeanActivity" => 0,
			 "MeanActivityRate" => 0,
			 "totalActivity" => 0,
			 "totalNbValues" => 0,
			 "AlarmDM" => isset($this->data["AlarmDM"]) ? $this->data["AlarmDM"] : 0,
			 "AlarmDR" => isset($this->data["AlarmDR"]) ? $this->data["AlarmDR"] : 0,
			 "tf" => 0 );
  }

  function update($cols)
  {
    $this->init_data();
    $this->data["date"] = $cols[0];
    foreach($this->cashes as $k => $v)
      {
	if($v)
	  {
	    $v->update($cols[$v->getIdx()]);
	  }
	if($v->isOpen())
	  {
	    $this->data["open"] += 1;
	  }
	else
	  {
	    $this->data["close"] += 1;
	  }
	if($v->isFull())
	  {
	    $this->data["full"] += 1;
	  }
	$this->data["alarmDM"] += $v->isAlarmDM();
	$this->data["alarmDR"] += $v->isAlarmDR();
	$this->data["total"] += $v->getValue();
	$this->data["totalActivity"] += $v->getTotalActivity();
	$this->data["totalNbValues"] += $v->getNbValues();
      }    

    $this->update_global();

    return $this->data;
  }
  
  protected function update_global()
  {    
    if($this->data["open"] > 0)
      {
	$this->data["MeanValue"] = round($this->data["total"] / $this->data["open"], 2);
	$this->data["MeanActivity"] = round($this->data["totalActivity"] / $this->data["open"], 2);
	$this->data["MeanActivityRate"] = round($this->data["totalActivity"] / $this->data["totalNbValues"], 2);
      }
    else
      {
	$this->data["MeanValue"] = 0;
	$this->data["MeanActivity"] = 0;
	$this->data["MeanActivityRate"] = 0;
      }
    if($this->data["MeanValue"] > $this->limitFull)
      {
	$this->tf += 1;
	if(!$this->data["AlarmDM"])
	  {
	    $this->data["AlarmDM"] = $this->tf == $this->maxTf ? 1 : 0;
	  }
	else
	  {
	    $this->data["AlarmDM"] = 0;
	  }
	if(!$this->data["AlarmDR"])
	  {
	    $this->data["AlarmDR"] = $this->tf == $this->maxTf2 ? 1 : 0;
	  }
	else
	  {
	    $this->data["AlarmDR"] = 0;
	  }
      }
    else
      {
	$this->tf = 0;
	$this->data["AlarmDM"] = 0;
	$this->data["AlarmDR"] = 0;
      }
    $this->data["tf"] = $this->tf;
  }

  function getCashesData()
  {
    $res = array();
    foreach($this->cashes as $k => $v)
      {
	$res[] = array( "id" => $v->getName(), 
			"value" => $v->getMeanValue(),
			"thresholdRate" => $v->getThresholdRate(),
			"open" => $v->getNbOpened(),
			"activity" => $v->getTotalActivity(),
			"arate" => $v->getActivityRate(),
			"tf" => $v->getTotalFull(),
			"maxTf" => $v->getMaxTf(),
			"alarmDM" => $v->getTotalAlarmDM(), 
			"alarmDR" => $v->getTotalAlarmDR() );
      }
    return $res;
  }
  
}

?>