<?php

include_once("BluePHP/BlueCountGUI/Config.inc");
include_once("BluePHP/BlueCountGUI/BCGUI_ModuleProcessing.inc");

class WaitingTimeGraphDayConfig extends BCGUI_ModuleProcessing
{
  function __construct($sharedCon = null)
  {
    parent::__construct(array( "name" => "WaitingTimeGraphDayConfig",
			       "keyname" => "id",
			       "sort" => "id" ),
			$sharedCon);
  }

  public function setEntry($params)
  {
    $params = array_merge( array( "withCalendar" => "0",
				  "withSelect" => "0" ),
			   $params );
    $str = "";
    $sep = "";
    foreach(parent::getModuleStructure() as $k => $v)
      {
	if(!is_numeric($params[$k]))
	  {
	    $str .= $sep . "'" . $params[$k] . "'";
	  }
	else
	  {
	    $str .= $sep . $params[$k];
	  }
	$sep = ",";
      }
    parent::QueryNoReturn("replace into WaitingTimeGraphDayConfig values(" . $str . ")");
    return $params;
  }

  public function newEntry($params)
  {
    $params = array_merge( $this->getDefaultParams(),
			   $params );
    return $this->setEntry($params);
  }

  public function getDefaultParams()
  {
    return array( "id" => 'null', 
		  "label" => "DEFAULT",
		  "title" => _("Daily wait time evolution for %date"),
		  "xtitle" => _("Values by minute"),
		  "ytitleN" => _("Number of persons"),
		  "ytitleWT" => _("Wait Time (minutes)"),
		  "ytitleF" => _("Pax/minutes"),
		  "tickInterval" => 60,
		  "serieColorWT" => "#4572A7",
		  "serieColorRWT" => "#AA4643",
		  "serieColorN" => "#5D8A59",
		  "serieColorF" => "#80699B",
		  "serieNameWT" => "Estimated wait time",
		  "serieNameRWT" => "Real wait time",
		  "serieNameN" => "Number of persons",
		  "serieNameF" => "Pax/minutes",
		  "withCalendar" => "0",
		  "withSelect" => "0",
		  "numId" => 0 );
  }
}

?>