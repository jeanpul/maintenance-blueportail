<?php

include_once("BluePHP/SmartMarket/Cash.inc");
include_once("BluePHP/SmartMarket/CashSet.inc");

class CashProcess
{
  var $hourLimits = array( "min" => 9, "max" => 21 );

  var $cashIdx = array();
  var $cashName = array();
  var $cashes = false;
  var $meanValues = array();
  var $dateValues = array();
  var $timeValues = array();
  var $totalValues = array();
  var $tfValues = array();
  var $timeAlarms = array();
  var $flowIn = array();
  var $openValues = array();
  var $closeValues = array();
  var $fullValues = array();
  var $flowModValues = array();
  var $maxValue = 0;
  var $totalMeanValue = 0;
  var $rateThreshold = 0;
  var $nbAlarmDM = 0;
  var $nbAlarmDR = 0;
  var $maxTf = 0;
  var $totalMeanActivity = 0;
  var $rateActivity = 0;
  var $nbValues = 0;
  var $maxFlowIn = 0;
  var $smFlowIn = array();
  var $smAlpha = 0.2;
  var $smPrev = 0;
  var $ratioValues = array();

  function CashProcess($cashName)
  {
    $this->cashName = $cashName;
  }

  function reset()
  {
    $this->meanValues = array();
    $this->dateValues = array();
    $this->timeValues = array();
    $this->tfValues = array();
    $this->timeAlarms = array();
    $this->totalValues = array();
    $this->flowModValues = array();
    $this->res = array();
    $this->flowIn = array();
    $this->maxValue = 0;
    $this->totalMeanValue = 0;
    $this->rateThreshold = 0;
    $this->nbAlarmDM = 0;
    $this->nbAlarmDR = 0;
    $this->maxTf = 0;
    $this->totalMeanActivity = 0;
    $this->rateActivity = 0;
    $this->nbValues = 0;
    $this->$smFlowIn = array();
    $this->$smAlpha = 1;
    $this->$smPrev = 0;
    $this->openValues = array();
    $this->closeValues = array();
    $this->fullValues = array();
    $this->ratioValues = array();
  }

  function getFlowModValues()
  {
    return $this->flowModValues;
  }

  function process($D)
  {
    // retrieve cols position/idx from cash name
    // and create the set of cashes
    $cols = array_map("trim", explode(",", $D[0]));
    $idxFlowIn = false;
    for($i = 0; $i < count($cols); $i++)
      {
	if(array_key_exists($cols[$i], $this->cashName))
	  {
	    $this->cashIdx[$cols[$i]] = new Cash($i, $this->cashName[$cols[$i]]);
	  }
	if($cols[$i] == "TOTAL_0")
	  {
	    $idxFlowIn = $i;
	  }
      }
    $this->cashes = new CashSet($this->cashIdx);
    $modVal = 0;
    for($i = 1; $i < count($D); $i++)
      {
	$cols = array_map("trim", explode(",", $D[$i]));
	$hour = (int) substr( $cols[0], -8, 2); // HH
	if($hour >= $this->hourLimits["min"] and
	   $hour <= $this->hourLimits["max"])
	  {
	    if($idxFlowIn)
	      {
		$this->flowIn[] = (int) $cols[$idxFlowIn];
		if($cols[$idxFlowIn] > $this->maxFlowIn)
		  {
		    $this->maxFlowIn = $cols[$idxFlowIn];
		  }
		if($i == 1)
		  {
		    $this->smPrev = (int) $cols[$idxFlowIn];
		    $this->smFlowIn[] = $this->smPrev;
		  }
		else
		  {
		    $value = $this->smAlpha * (int) $cols[$idxFlowIn] + (1 - $this->smAlpha) * $this->smPrev;
		    $this->smFlowIn[] = $value;
		    $this->smPrev = $value;
		  }
		if($i % 15)
		  {
		    $this->flowModValues[] = null;
		    $modVal += (int) $cols[$idxFlowIn];
		      }
		else
		  {
		    $this->flowModValues[] = $modVal;
		    $modVal = 0;
	      }
	      }

	    $row = $this->cashes->update($cols);
	    $this->nbValues += 1;
	    $this->dateValues[] = $row["date"];
	    $this->timeValues[] = substr( $row["date"], -8, 5); // HH:mm
	    $this->meanValues[] = $row["MeanValue"];
	    $this->tfValues[] = $row["tf"];
	    $this->openValues[] = $row["open"];
	    $this->closeValues[] = $row["close"];
	    $this->fullValues[] = $row["full"];
	    $this->totalValues[] = $row["total"];
	    $this->ratioValues[] = $row["open"] ? round($row["full"] / $row["open"], 2) : 0;
	    if($row["MeanValue"] > $this->maxValue)
	      {
		$this->maxValue = $row["MeanValue"];
	      }

	    $this->totalMeanValue += $row["MeanValue"];

	    if($row["MeanValue"] <= $this->getThreshold())
	      {
		$this->rateThreshold += 1;
	      }

	    $this->nbAlarmDR += $row["AlarmDR"];
	    $this->nbAlarmDM += $row["AlarmDM"];

	    if($row["tf"] > $this->maxTf)
	      {
		$this->maxTf = $row["tf"];
	      }
	    $this->totalMeanActivity += $row["open"];
	  }
      }
    if(count($this->meanValues))
      {
	$this->totalMeanValue = round($this->totalMeanValue / count($this->meanValues), 2);
	$this->rateThreshold = round($this->rateThreshold / count($this->meanValues) * 100, 2); 
      }
    if(count($this->cashIdx))
      {
	$this->totalMeanActivity = round($this->totalMeanActivity / count($this->cashIdx), 0);	
      }
    if($this->nbValues > 0)
      {
	$this->rateActivity = round($this->totalMeanActivity / $this->nbValues * 100, 0);
      }
  }

  function getRatioValues()
  {
    return $this->ratioValues;
  }

  function getOpenValues()
  {
    return $this->openValues;
  }

  function getCloseValues()
  {
    return $this->closeValues;
  }

  function getFullValues()
  {
    return $this->fullValues;
  }

  function getTotalValues()
  {
    return $this->totalValues;
  }

  function getNbAlarmDR()
  {
    return $this->nbAlarmDR;
  }

  function getNbAlarmDM()
  {
    return $this->nbAlarmDM;
  }

  function getTotalMeanValue()
  {
    return $this->totalMeanValue;
  }

  function getThresholdRate()
  {
    return $this->rateThreshold;
  }

  function getMeanValues() 
  {
    return $this->meanValues;
  }

  function getFlowInValues()
  {
    return $this->flowIn;
  }

  function getSmFlowInValues()
  {
    return $this->smFlowIn;
  }

  function getMaxFlowIn()
  {
    return $this->maxFlowIn;
  }

  function getDateValues()
  {
    return $this->dateValues;
  }

  function getTimeValues()
  {
    return $this->timeValues;
  }

  function getMaxValue()
  {
    return $this->maxValue;
  }

  function getThreshold()
  {
    return 2;
  }

  function getAlarmFirstThreshold()
  {
    return 5;
  }

  function getAlarmSecondThreshold()
  {
    return 15;
  }

  function getMaxTf()
  {
    return $this->maxTf;
  }

  function getTfValues()
  {
    return $this->tfValues;
  }

  function getCashesData()
  {
    return $this->cashes->getCashesData();
  }

  function getTotalMeanActivity()
  {
    return $this->totalMeanActivity;
  }

  function getActivityRate()
  {
    return $this->rateActivity;
  }

  function getNbCashes()
  {
    return count($this->cashIdx);
  }
}

?>
