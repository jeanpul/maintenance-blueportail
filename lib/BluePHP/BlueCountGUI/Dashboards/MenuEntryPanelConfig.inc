<?php

include_once("BluePHP/BlueCountGUI/Config.inc");
include_once("BluePHP/BlueCountGUI/BCGUI_ModuleProcessing.inc");

/**
 * \class MenuEntryPanelConfig
 */
class MenuEntryPanelConfig extends BCGUI_ModuleProcessing
{
  function __construct($sharedCon = null)
  {
    parent::__construct(array( "name" => "MenuEntryConfig",
			       "keyname" => "id",
			       "sort" => "id" ),
			$sharedCon);
  }

  public function setEntry($params)
  {
    parent::QueryNoReturn("replace into MenuEntryConfig values(" . $params["id"] . ",'" . 
			  $params["icon"] . "','" . $params["label"] . "','" . 
			  $params["href"] . "')");
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
		  "icon" => "default",
		  "href" => "" );
  }
}

?>