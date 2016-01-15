<?php

include_once("IndicatorVisitors.inc");
include_once("IndicatorTitle.inc");
include_once("PeriodTitleFlowXMLChart.inc");
include_once("TabularWaitingTimeXMLChart.inc");

class IndicatorWaitingTime extends IndicatorVisitors
{

  // waiting time from numbering
  protected $numberToTime = array( array (  0, 5 ),
				   array (  5, 10 ),
				   array ( 10, 15 ),
				   array ( 15, 0 ) );

  function IndicatorWaitingTime($glang, $params)
    {
      parent::IndicatorVisitors($glang, $params);
      $this->name = "Mean waiting time";
      $this->colKeys = array( "period", "value" );
      $this->rfunc = "getWaitingTimeIds";
      
      $this->displayPeriods = array( 'hour' => array('minute' => "Hour / 1 minute"),
				     'day' => array('minute' => "Day / 1 minute")
				     );
    }

  function setAnalysisTitles()
    {
      global $userEntities;

      $this->indicatorTitle = new IndicatorTitle($this->params["Step"]);

      if(!isset($this->params["id"]))
	{
	  $this->params["entName"] = _("Unknown");
	  $this->indicatorTitle->disableGeneralTitle();
	}
      else
	{
	  $this->params["entName"] = $userEntities->getLocationName($this->params["id"]);
	  $this->indicatorTitle->setGeneralTitle(_($this->name),
						 _("for location"),
						 $this->params["entName"],
						 $this->params["timestamp"]);
	}

      $this->indicatorTitle->setValuesTitles( array( _("Periods"), 
						     _($this->name) ));
    }
  
  function computeCountersValues()
    {
      // REPORT PARAMETERS
      if(is_array($this->counters) and count($this->counters))
	{
	  // PROCESS
	  $paramsCounters = $this->params;
	  $paramsCounters["TimeStamp"] = $this->params["timestamp"];
	  $paramsCounters["idp"] = $this->counters[$this->params["id"]]["idp"];

	  // source parameters
	  $this->numberToTime = $this->glang->getIndicatorParams($paramsCounters);
	  $this->values = $this->glang->clang->getWaitingTimeValues($paramsCounters);

	  for($i = 0; $i < count($this->values); $i++)
	    {
	      $this->values[$i] = $this->getStrWaitingTimeValue($this->values[$i]);
	    }
	}
    }
  
  function getStrAccordingStep($idx)
  {
    if($idx == 0)
      {
	$str = _("less than %d minutes");
	return sprintf($str, $this->numberToTime[$idx][1]);
      }
    else if($idx == (count($this->numberToTime) - 1))
      {
	$str = _("more than %d minutes");
	return sprintf($str, $this->numberToTime[$idx][0]);
      }
    else
      {
	$str = _("between %d and %d minutes");
	return sprintf($str, $this->numberToTime[$idx][0], $this->numberToTime[$idx][1]);
      }
  }

  function getStrWaitingTimeValue($elt)
    {      
      $strValue = "";
      for($i = 0; $i < count($this->numberToTime); $i++)
	{
	  if($elt["value"] >= $this->numberToTime[$i][0] and
	     ($elt["value"] < $this->numberToTime[$i][1] or
	      $this->numberToTime[$i][1] == 0))
	    {
	      $strValue = $this->getStrAccordingStep($i);
	      $elt["val" . $i] = 2;
	    }
	  else
	    {
	      $elt["val" . $i] = 0;
	    }
	}
      $elt["value"] = $strValue;
      return $elt;
    }

  function getGraphicContents()
    {
      // change period titles
      $periodTitle = new PeriodTitleFlowXMLChart($this->params["Step"]);
      for($i = 0; $i < count($this->values); $i++)
	{
	  $this->values[$i]["period"] = $periodTitle->getPeriodTitle($this->values[$i]);
	}
      
      $strsteps = array("");
      $keysteps = array("period");
      for($idx = 0; $idx < count($this->numberToTime); $idx++)
	{
	  array_push($strsteps,  $this->getStrAccordingStep($idx));
	  array_push($keysteps, "val" . $idx);
	}
      $tabular = new TabularWaitingTimeXMLChart($strsteps,
						$this->values, 
						$keysteps);
      
      return getGraphicalBox($this->getId(), $this->indicatorTitle->getGeneralTitle(), 
			     getGraphicPairTable($tabular->getData(array("id" => 0, "key" => $this->key)), 
						 ""));
    }
}

?>
