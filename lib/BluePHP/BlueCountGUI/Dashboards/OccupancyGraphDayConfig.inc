<?php

include_once("BluePHP/BlueCountGUI/Config.inc");
include_once("BluePHP/BlueCountGUI/BCGUI_ModuleProcessing.inc");

class OccupancyGraphDayConfig extends BCGUI_ModuleProcessing
{
  function __construct($sharedCon = null)
  {
    parent::__construct(array( "name" => "OccupancyGraphDayConfig",
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
    parent::QueryNoReturn("replace into OccupancyGraphDayConfig values(" . $str . ")");
    return $params;
  }

  public function newEntry($params)
  {
    $params = array_merge( $this->getDefaultParams(),
			   $params );
    $this->setEntry($params);

  }

  public function getDefaultParams()
  {
    return array( "id" => 'null', 
		  "label" => "DEFAULT",
		  "title" => _("Evolution of the number of people"),
		  "xtitle" => _("Values by minute"),
		  "ytitle" => _("Number of people"),
		  "tickInterval" => 60,
		  "serieColor" => "#5D8A59",
		  "serieName" => "Occupancy",
		  "dateFmt" => "%m/%d/%Y",
		  "withCalendar" => "0",
		  "withSelect" => "0",
		  "numId" => 0 );
  }
}

?>