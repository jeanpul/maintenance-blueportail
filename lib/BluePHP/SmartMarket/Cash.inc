<?php

class Cash
{
  var $idx = false;
  var $name = "NONAME";
  var $open = false;
  var $to = 0;
  var $maxTo = 0;
  var $value = 0;
  var $limitFull = 2;
  var $tf = 0;
  var $maxTf = 0; 
  var $totalValue = 0;
  var $nbOpen = 0;
  var $totalTo = 0;
  var $totalFull = 0;
  var $nbValue = 0;
  var $alarmDM = 0;
  var $alarmDR = 0;
  var $totalAlarmDM = 0;
  var $totalAlarmDR = 0;

  function Cash($idx, $name, $maxTo = 1)
    {
      $this->idx = $idx;
      $this->name = $name;
      $this->maxTo = $maxTo;
    }

  function update($value)
  {
    if($value != -1)
      {
	$this->value = $value;
	if($value == 0)
	  {
	    if($this->to > 0)
	      {
		$this->to -= 1;
	      }
	  }
	else if($value > 0) 
	  {
	    $this->to = min($this->maxTo, $this->to + 1);
	    $this->totalTo += 1;

	    if(!$this->open)
	      {
		$this->nbOpen += 1;
	      }
	  }
	$this->open = ($this->to > 0);
	if($this->isFull())
	  {
	    $this->tf += 1;
	    $this->totalFull += 1;
	  }
	else
	  {
	    $this->tf = 0;
	  }
	if($this->tf > $this->maxTf)
	  {
	    $this->maxTf = $this->tf;
	  }
	$this->nbValue += 1;
	$this->totalValue += $value;

	if(!$this->alarmDM)
	  {
	    $this->alarmDM = $this->tf >= 5 ? 1 : 0;
	  }
	else
	  {
	    $this->alarmDM = 0;
	  }
	if(!$this->alarmDR)
	  {
	    $this->alarmDR = $this->tf >= 15 ? 1 : 0;
	  }
	else
	  {
	    $this->alarmDR = 0;
	  }
	$this->totalAlarmDM += $this->alarmDM;
	$this->totalAlarmDR += $this->alarmDR;
      }
  }

 function isOpen()
  {
    return $this->open;
  }

  function getIdx()
  {
    return $this->idx;
  }
  
  function getName()
  {
    return $this->name;
  }

  function getValue()
  {
    return $this->value;
  }

  function getTotalValue()
  {
    return $this->totalValue;
  }

  function getMeanValue()
  {
    return $this->totalTo ? round($this->totalValue / $this->totalTo, 2) : 0;
  }

  function getNbValues()
  {
    return $this->nbValue;
  }

  function isFull()
  {
    return $this->getValue() > $this->limitFull;
  }

  function isAlarmDM()
  {
    return $this->alarmDM;
  }

  function isAlarmDR()
  {
    return $this->alarmDR;
  }

  function getTotalAlarmDM()
  {
    return $this->totalAlarmDM;
  }

  function getTotalAlarmDR()
  {
    return $this->totalAlarmDR;
  }

  function getNbOpened()
  {
    return $this->nbOpen;
  }

  function getTotalActivity()
  {
    return $this->totalTo;
  }
  
  function getActivityRate()
  {    
    return $this->getTotalValue() ? round($this->getTotalActivity() / $this->getNbValues() * 100) : 0;
  }

  function getTotalFull()
  {
    return $this->totalFull;
  }

  function getMaxTf()
  {
    return $this->maxTf;
  }

  function getThresholdRate()
  {
    return $this->getTotalActivity() ?  round((1 - ($this->getTotalFull() / $this->getTotalActivity())) * 100, 2) : 0;
  }

}

?>
