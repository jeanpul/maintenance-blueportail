<?php

include_once("BluePHP/BTopLocalServer/Config.inc");
include_once("BluePHP/Utils/ModuleProcessing.inc");

class TimeZone extends ModuleProcessing
{
  public function __construct($sharedCon = null)
  {
    parent::__construct(array( "name" => "TimeZone",
			       "keyname" => "tz",
			       "sort" => "tz" ),
			$sharedCon);
  }

  function getEntry($params)
  {
    return $this->Select("tz from TimeZone");
    
  }

  function setEntry($params)
  {
    $this->QueryNoReturn("update TimeZone set tz=\"" . $params["tz"] . "\"");
    return true;
  }

  function getTimeZones()
  {
    return timezone_identifiers_list();
  }
}

?>