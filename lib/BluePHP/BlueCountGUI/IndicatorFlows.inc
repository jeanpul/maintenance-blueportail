<?php

include_once("IndicatorVisitors.inc");
include_once("IndicatorTitle.inc");
include_once("PeriodTitleFlowXMLChart.inc");
include_once("TabularMonoValue.inc");
include_once("TabularFlowXMLChart.inc");

class IndicatorFlows extends IndicatorVisitors
{
  function IndicatorFlows($glang, $params)
    {
      parent::IndicatorVisitors($glang, $params);

      $this->name = "Counts";
      $this->colKeys = array( "period", "value0", "value1" );
      $this->entities = array( "door" => "Door", "group" => "Group" );
    }

  function setAnalysisTitles()
    {
      global $userEntities;

      $this->params["entName"] = $userEntities->getDoorName($this->params["id"]);
      $this->params["Z1Name"] = $userEntities->getLocationName($this->counters[$this->params["id"]]["idZ1"]);
      $this->params["Z2Name"] = $userEntities->getLocationName($this->counters[$this->params["id"]]["idZ2"]);

      $this->indicatorTitle = new IndicatorTitle($this->params["Step"]);
      $this->indicatorTitle->setGeneralTitle(_($this->name),
					     _("for door"),
					     $this->params["entName"],
					     $this->params["timestamp"]);

      if(isset($this->params["TitleMode"]) and $this->params["TitleMode"] == "simple")
	{
	  $this->indicatorTitle->setValuesTitles( array( _("Periods"), 
							 $this->params["Z1Name"],
							 $this->params["Z2Name"]) );
	}
      else
	{
	  $this->indicatorTitle->setValuesTitles( array( _("Periods"), 
							 $this->params["Z1Name"] . " " . _("to") . " " . $this->params["Z2Name"],				     
							 $this->params["Z2Name"] . " " . _("to") . " " . $this->params["Z1Name"]));
	}
    }

  protected function computeCountersValues()
    {
      // REPORT PARAMETERS
      if(is_array($this->counters) and count($this->counters))
	{
	  // PROCESS
	  $paramsCounters = $this->params;
	  $paramsCounters["TimeStamp"] = $this->params["timestamp"];
	  $paramsCounters["idp"] = $this->counters[$this->params["id"]]["idp"];

	  $this->values = $this->glang->clang->getCountersValues($paramsCounters);
	}
    }

  function getGraphicContents()
    {
      // change period titles
      $periodTitle = new PeriodTitleFlowXMLChart($this->params["Step"]);
      for($i = 0; $i < count($this->values); $i++)
	{
	  $this->values[$i]["period"] = $periodTitle->getPeriodTitle($this->values[$i]);
	}

      $tabularIn = new TabularFlowXMLChart(array( "", $this->indicatorTitle->getValuesTitle(1) ), 
					   $this->values, 
					   array( "period", "value0" ));

      $tabularOut = new TabularFlowXMLChart(array( "", $this->indicatorTitle->getValuesTitle(2) ), 
					    $this->values, 
					    array( "period", "value1" ));
      
      return getGraphicalBox($this->getId(), $this->indicatorTitle->getGeneralTitle(), 
			     getGraphicPairTable($tabularIn->getData(array("id" => 0, "key" => $this->key)), 
						 "",
						 $tabularOut->getData(array("id" => 1, "key" => $this->key)), 
						 ""));
    }
}

?>
