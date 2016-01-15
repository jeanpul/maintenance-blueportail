<?php

include_once("BluePHP/BlueCountGUI/Config.inc");
include_once("BluePHP/BlueCountGUI/BCGUI_ModuleProcessing.inc");

class CameraViewConfig extends BCGUI_ModuleProcessing
{
  function __construct($sharedCon = null)
  {
    parent::__construct(array( "name" => "CameraViewConfig",
			       "keyname" => "id",
			       "sort" => "id" ),
			$sharedCon);
  }

  public function setEntry($params)
  {
    parent::QueryNoReturn("replace into CameraViewConfig values(" . $params["id"] . ",'" . 
			  $params["label"] . "','" . $params["URL"] . "','" . 
			  $params["login"] . "','" . $params["passwd"] . "'," . 
			  $params["refreshRate"] . ")");
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
		  "URL" => "",
		  "login" => "",
		  "passwd" => "",
		  "refreshRate" => 60 );
  }
}

?>
