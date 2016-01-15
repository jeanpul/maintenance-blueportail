<?php

include_once("BluePHP/BTopLocalServer/Config.inc");
include_once("BluePHP/Utils/ModuleProcessing.inc");

class Calendar extends ModuleProcessing
{
  public function __construct($sharedCon = null)
  {
    parent::__construct(array( "name" => "CalData",
			       "keyname" => "id",
			       "sort" => "id" ),
			$sharedCon);
  }

  public function getCalendars()
  {
    return $this->Select("Calendars.id as calId, Calendars.name as calName, " . 
			 "testts, ts, testte, te from Calendars, CalData where " .
			 "Calendars.id = CalData.cal order by calId, pri");
  }
}

?>