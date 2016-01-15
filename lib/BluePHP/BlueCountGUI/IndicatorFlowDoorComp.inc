<?php

include_once("IndicatorTitle.inc");
include_once("PeriodTitleFlowXMLChart.inc");
include_once("TabularMonoValue.inc");
include_once("TabularFlowXMLChart.inc");
include_once("IndicatorFlows.inc");

class IndicatorFlowDoorComp extends IndicatorFlows
{
  function IndicatorFlowDoorComp($glang, $params)
    {
      parent::IndicatorFlows($glang, $params);
      $this->colkeys = array( "period" ); 
      $this->entities = array( "group" => "Group" );
      $this->name = "Doors flows comparisons";
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
	  $this->params["entName"] = $userEntities->getGroupName($this->params["id"]);
	  $this->indicatorTitle->setGeneralTitle(_($this->name),
						 _("for the group"),
						 $this->params["entName"],
						 $this->params["timestamp"]);
	}
    }

  function computeCountersValues()
    {
      $this->values = array();

      // REPORT PARAMETERS
      //      if(is_array($this->counters) and count($this->counters))
      //	{
//	  // PROCESS
//	  $paramsCounters = $this->params;
//
//	  // get all the objects belonging to the counter id
//	  $doors = $this->glang->clang->getCountingObjectsIdx($paramsCounters);
//	  
//	  $paramsCounters["Entity"] = "door";
//
//	  // for each object compute the counting values
//	  // fab: this could be optimized if everything is done
//	  // into the SQL statement
//	  $this->values = array();
//	  foreach($doors as $k => $v)
//	    {
//	      $paramsCounters["idp"] = $v["idpD"];
//	      $paramsCounters["TimeStamp"] = $this->params["timestamp"];
//	      $data = $glang->clang->getCountersValues($paramsCounters);
//	      if(count($data))
//		{
//		  $this->values[$k] = $data;
//		}
//	    }
//	}
    }
}

?>
