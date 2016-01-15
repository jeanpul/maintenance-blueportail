<?php

include_once("BluePHP/BTopLocalServer/Config.inc");
include_once("BluePHP/Utils/ModuleProcessing.inc");

class NumberingAvailable extends ModuleProcessing
{
  function __construct($sharedCon = null)
  {
    parent::__construct(array( "name" => "NumberingAvailable",
			       "keyname" => "id",
			       "sort" => "id" ),
			$sharedCon);    
  }

  function getEntriesFromRef($params)
  {
    return parent::Select("* from NumberingAvailable where ref=\"" . $params["ref"] . "\"");
  }
}

?>