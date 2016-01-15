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

  public function createCalendar($params)
  {
    if(!isset($params["id"]))
      {
	$newCalId = 0;
	$cals = $this->Select("id from Calendars order by id desc limit 1");
	if(count($cals) > 0)
	  {
	    $newCalId = $cals[0]["id"] + 1;
	  }
	$params["id"] = $newCalId;
      }
    parent::QueryNoReturn("insert into Calendars values($newCalId, '" .
			  $params["name"] . "')");
    return $params;
  }

  public function addCalendarRule($params)
  {
  }
}

?>