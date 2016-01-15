<?php

include_once("BluePHP/BlueCountGUI/Config.inc");
include_once("BluePHP/BlueCountGUI/BCGUI_ModuleProcessing.inc");

class FlowHistoryConfig extends BCGUI_ModuleProcessing
{
  function __construct($sharedCon = null)
  {
    parent::__construct(array( "name" => "FlowHistoryConfig",
			       "keyname" => "id",
			       "sort" => "id" ),
			$sharedCon);
  }

  public function setEntry($params)
  {
    return parent::replaceEntry($params);
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
		  "label" => "%nameF",
		  "numId" => null,
		  "length" => 5,
		  "serieColor" => "#4e688e",
		  "field" => "value0" );
  }
}

?>