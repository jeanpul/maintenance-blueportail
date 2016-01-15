<?php

include_once("IndicatorTitle.inc");
include_once("PeriodTitleFlowXMLChart.inc");
include_once("TabularMonoValue.inc");
include_once("TabularFlowXMLChart.inc");
include_once("IndicatorVisitors.inc");

class IndicatorNumberingLocComp extends IndicatorVisitors
{
  function IndicatorNumberingLocComp($glang, $params)
    {
      parent::IndicatorVisitors($glang, $params);
      $this->colkeys = array( "period" ); 
      $this->entities = array( "area" => "Area" );
      $this->name = "Location numbering comparisons";
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
	  $this->params["entName"] = $userEntities->getAreaName($this->params["id"]);
	  $this->indicatorTitle->setGeneralTitle(_($this->name),
						 _("for the area"),
						 $this->params["entName"],
						 $this->params["timestamp"]);
	}
    }

  function computeCountersValues()
    {
      // REPORT PARAMETERS
      if(is_array($this->counters) and count($this->counters))
	{
//	  // PROCESS
//	  $paramsCounters = $this->params;
//	  
//	  // get all the objects belonging to the counter id
//	  $locations = $clang->getNumberingObjectsIdx($paramsCounters);
//	  
//	  $paramsCounters["Entity"] = "location";
//	  
//	  // for each object compute the counting values
//	  // fab: this could be optimized if everything is done
//	  // into the SQL statement
//	  $this->values = array();
//	  foreach($locations as $k => $v)
//	    {
//	      $paramsCounters["idp"] = $v["idpL"];
//	      $paramsCounters["TimeStamp"] = $this->params["timestamp"];
//	      $data = $this->clang->getNumberingValues($paramsCounters);
//	      if(count($data))
//		{
//		  $this->values[$k] = $data;
//		}  
//	    }	  
	}
    }
}

?>
