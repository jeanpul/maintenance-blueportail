<?php

include_once("IndicatorVisitors.inc");

class IndicatorOccupancy extends IndicatorVisitors
{
  function IndicatorOccupancy($glang, $params)
    {
      parent::IndicatorVisitors($glang, $params);
      $this->colKeys = array( "period", "occ" );
    }

  function setAnalysisTitles()
    {
      global $userEntities;

      $this->params["entName"] = $userEntities->getLocationName($this->params["id"]);
      $this->indicatorTitle = new IndicatorTitle($this->params["Step"]);
      $this->indicatorTitle->setGeneralTitle(_("Mean number of persons"),
					     _("for location"),
					     $this->params["entName"],
					     $this->params["timestamp"]);
      $this->indicatorTitle->setValuesTitles( array( _("Periods"), 
						     _("Mean number of persons") ));
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
	  $this->values = $this->glang->clang->getCountersValues($paramsCounters);
	  
	  if(is_array($this->values) && count($this->values))
	    {
	      $prev = 0;
	      foreach($this->values as $idx => $v)
		{		
		  $occ = $prev + $this->values[$idx]["value1"] - $this->values[$idx]["value0"];
		  $this->values[$idx]["occ"] = $occ;
		  $prev = $occ;	      
		}
	    }
	}
    }
}

?>
