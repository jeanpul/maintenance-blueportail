<?php

include_once("BluePHP/Utils/DateOps.inc");
include_once("BluePHP/BTopLocalServer/Config.inc");
include_once("BluePHP/Utils/ModuleProcessing.inc");
include_once("BluePHP/BTopLocalServer/BlueCountLang.inc");

class ZoneCounting extends ModuleProcessing
{
  private $clang = false;

  function __construct($sharedCon = null)
  {
    parent::__construct(array( "name" => "ZoneCountingProcessing",
			       "keyname" => "id",
			       "sort" => "id" ),
			$sharedCon);
    $this->clang = new BlueCountLang();
  }

  function getCalendars()
  {
    return parent::Select("* from Calendars");
  }

  function getZones()
  {
    return $this->clang->getZones(array("joinedCountingProcessing" => true));
  }

  function getFunctions()
  {
    return parent::Select("* from ZoneCountingType");
  }

  function getEntries()
  {
    return $this->clang->getZoneCountingProcessing();
  }

  public function setCalendar($zcpId, $calId)
  {
    parent::QueryNoReturn("update ZoneCountingProcessing set calendar=$calId where id=$zcpId");
  }

  function getEntryFromZone($params)
  {
    $res = array();
    $elts = parent::Select("* from ZoneCountingProcessing where zone=\"" . $params["zone"] . "\"");
    if(is_array($elts) and count($elts))
      {
	$res = $elts[0];
      }
    return $res;
  }

  function getOneMinuteValues($params)
  {
    $params["Step"] = "minute";
    $params["CompPeriod"] = "minute";
    $params["Entity"] = "location";
    $params["idp"] = $params["id"];
    return $this->clang->getVisitorsValues($params);
  }

  function getValues($params)
  {
    $params["idp"] = $params["id"];
    return $this->clang->getVisitorsValues($params);
  }
  
  function getZonesFromCounter($ref)
  {
    return parent::Select("ZoneCountingProcessing.id as cpId, " .
			  "ZoneCountingProcessing.id as zcpId, Zones.name as zcpName, Zones.name as cpName, " .
              "Calendars.name as calName, Calendars.id as calId, Flows.zone1, Flows.zone2" . 
			  " from Zones, Flows, Calendars, ZoneCountingProcessing, CountersAssoc, CountersAvailable" .
              " where clientId='$ref' and CountersAvailable.id=CountersAssoc.counter and CountersAssoc.flow=Flows.id and" .
			  " (Zones.id=Flows.zone1 or Zones.id=Flows.zone2) and ZoneCountingProcessing.zone=Zones.id and ZoneCountingProcessing.calendar=Calendars.id" . 
			  " group by zcpId order by zcpId");
  }

  public function getIndicator($cpId)
  {
    include_once("indicators/ZoneCounting_flow.inc");
    return new ZoneCounting_flow($this->clang);
  }
}

?>
