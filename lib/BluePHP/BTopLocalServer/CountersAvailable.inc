<?php

include_once("BluePHP/BTopLocalServer/Config.inc");
include_once("BluePHP/Utils/ModuleProcessing.inc");

class CountersAvailable extends ModuleProcessing
{
  function __construct($sharedCon = null)
  {
    parent::__construct(array( "name" => "CountersAvailable",
			       "keyname" => "id",
			       "sort" => "id" ),
			$sharedCon);    
  }

  function getEntriesFromRef($params)
  {
    return parent::Select("* from CountersAvailable where ref=\"" . $params["ref"] . "\"");
  }
}

?>