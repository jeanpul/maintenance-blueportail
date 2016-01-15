<?php

include_once("BluePHP/BlueCountGUI/Config.inc");
include_once("BluePHP/BlueCountGUI/BCGUI_ModuleProcessing.inc");

class MapImageConfig extends BCGUI_ModuleProcessing
{
  public function __construct($sharedCon = null)
  {
    parent::__construct(array( "name" => "MapImageConfig",
			       "keyname" => "id",
			       "sort" => "id" ),
			$sharedCon);
  }

  public function getDefaultParams()
  {
    return array( "id" => 'null',
		  "src" => '',
		  "alt" => '' );
  }
}

?>
