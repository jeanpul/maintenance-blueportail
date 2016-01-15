<?php

include_once("IndicatorVisitors.inc");
include_once("PeriodTitleFlowXMLChart.inc");
include_once("TabularFlowXMLChart.inc");
include_once("TabularMeanXMLChart.inc");
include_once("TabularMonoValue.inc");

class IndicatorNumbering extends IndicatorVisitors
{
  function IndicatorNumbering($glang, $params)
    {
      parent::IndicatorVisitors($glang, $params);        
      if(isset($this->params["Step"]) and ($this->params['Step'] == 'minute' || $this->params['Step'] == '1minute'))
	{ 
	  $this->name = "Number of persons";
	  $this->colKeys = array( "period", "value");
	}
      else
	{ 
	  $this->name = "Mean number of persons";
	  $this->colKeys = array( "period", "value", "stddev", "minvalue", "maxvalue" );
	}
      $this->rfunc = "getNumberingIds";
     
      $this->displayPeriods = array( 'hour' => array('minute' => "Hour / 1 minute",
						     '5minute' => "Hour / 5 minutes"),
				     'day' => array('1minute' => "Day / 1 minute",
						    '15minute' => "Day / 15 minutes",
						    'hour' => "Day / hour")
						    
						    
				     );
    }

  function setAnalysisTitles()
    {
      global $userEntities;

      $this->params["entName"] = $userEntities->getLocationName($this->params["id"]);
      $this->indicatorTitle = new IndicatorTitle($this->params["Step"]);
      $this->indicatorTitle->setGeneralTitle(_($this->name),
					     _("for location"),
					     $this->params["entName"],
					     $this->params["timestamp"]);
      if($this->params['Step'] == 'minute' || $this->params['Step'] == '1minute')
	{	  
	  $this->indicatorTitle->setValuesTitles( array( _("Periods"), 
							 _($this->name)));
	}
      else
	{
	  $this->indicatorTitle->setValuesTitles( array( _("Periods"), 
							 _($this->name),
							 _("std dev."),
							 _("min"),
							 _("max")));
	}
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
	  $this->values = $this->glang->clang->getNumberingValues($paramsCounters);
	}
    }

  function getTabularContents()
    {
      $periodTitle = new PeriodTitle($this->params["Step"]);
      for($i = 0; $i < count($this->values); $i++)
	{
	  $this->values[$i]['stddev'] = sqrt($this->values[$i]['variance']);
	  $this->values[$i]["period"] = $periodTitle->getPeriodTitle($this->values[$i]);
	}

      $tabular = new TabularMonoValue($this->indicatorTitle->getValuesTitles(),
				      $this->values, 
				      $this->colKeys);
      
      return getTabularBox($this, 
			   $this->indicatorTitle->getGeneralTitle(), 
			   $tabular->getData());
    }

  function getGraphicContents()
    {
      // change period titles
      if($this->params['Step'] == 'minute' || $this->params['Step'] == '1minute')
	{
	  $periodTitle = new PeriodTitleFlowXMLChart($this->params["Step"]);
	  for($i = 0; $i < count($this->values); $i++)
	    {
	      $this->values[$i]["period"] = $periodTitle->getPeriodTitle($this->values[$i]);
	    }
	  
	  $tabular = new TabularFlowXMLChart(
					     array("", $this->indicatorTitle->getSubTitle()),
					     $this->values, 
					     array( "period", "value"));
	  
	  return getGraphicalBox($this->getId(), $this->indicatorTitle->getGeneralTitle(), 
				 getGraphicPairTable($tabular->getData(array("id" => 0, "key" => $this->key)), 
						     ""));
	}
      else
	{
	  $periodTitle = new PeriodTitleFlowXMLChart($this->params["Step"]);
	  for($i = 0; $i < count($this->values); $i++)
	    {
	      $this->values[$i]['stddev'] = sqrt($this->values[$i]['variance']);
	      $this->values[$i]["period"] = $periodTitle->getPeriodTitle($this->values[$i]);
	    }
	  
	  $tabular = new TabularMeanXMLChart(
					     array("", $this->indicatorTitle->getSubTitle(),
						   /*"std. dev.",*/ "min", "max"),
					     //array( "", $this->indicatorTitle->getSubTitle() ), 
					     $this->values, 
					     array( "period", "value", /*"stddev",*/ "minvalue", "maxvalue" )
					     //array( "period", "value", "minvalue", "maxvalue" )
					     );
	  
	  return getGraphicalBox($this->getId(), $this->indicatorTitle->getGeneralTitle(), 
				 getGraphicPairTable($tabular->getData(array("id" => 0, "key" => $this->key)), 
						     ""));
	}
    }
}

?>
