<?php

include_once("BluePHP/BlueCountGUI/Config.inc");
include_once("BluePHP/BlueCountGUI/BCGUI_ModuleProcessing.inc");

class OccupancyConfig extends BCGUI_ModuleProcessing
{
  function __construct($sharedCon = null)
  {
    parent::__construct(array( "name" => "OccupancyConfig",
			       "keyname" => "id",
			       "sort" => "id" ),
			$sharedCon);
  }

  public function setEntry($params)
  {
    parent::QueryNoReturn("replace into OccupancyConfig values(" . $params["id"] . ",'" . 
			  $params["label"] . "'," . $params["numId"] . ")");
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
    return array( "id" => 'null', // 'null' will automatically increments the widget id
		  "label" => "%nameZ",
		  "numId" => 0 );
  }
}

?>