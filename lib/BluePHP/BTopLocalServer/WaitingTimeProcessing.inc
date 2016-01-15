<?php

include_once("BluePHP/BTopLocalServer/Config.inc");
include_once("BluePHP/Utils/ModuleProcessing.inc");
include_once("BluePHP/BTopLocalServer/BlueCountLang.inc");
include_once("BluePHP/BTopLocalServer/GlobalProperties.inc");

class WaitingTimeProcessing extends ModuleProcessing
{
  private $clang = false;

  function __construct($sharedCon = null)
  {
    parent::setParams( array( "name" => "ZoneWaitingTimeProcessing",
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
    return $this->clang->getZonesWithNumberingProcessing();
  }

  function getFunctions()
  {
    return parent::Select("* from WaitingTimeType");
  }

  public function haveType($typeName)
  {
    $data = parent::Select("count(*) as c from ZoneWaitingTimeProcessing, WaitingTimeType where " . 
			   "wt_id=WaitingTimeType.id and name='$typeName'");
    return (int) $data[0]["c"];
  }

  function getEntries()
  {
    return $this->clang->getZoneWaitingTimeProcessing();
  }

  function getOneMinuteValues($params)
  {
    if(!isset($params["TimeStr"]))
      {
	$module = new GlobalProperties();
	$params["TimeStr"] = $module->getLastProcessedTimeString();
      }

    $params["Step"] = "minute";
    $params["CompPeriod"] = "minute";
    $params["Entity"] = "area";
    $params["idp"] = $params["id"];
    return $this->clang->getWaitingTimeValues($params);
  }

  function getWaitingTimeValues($params)
  {
    if(!isset($params["idp"]) and isset($params["id"]))
      {
	$params["idp"] = $params["id"];
      }
    return $this->clang->getWaitingTimeValues($params);
  }

  public function getProcessingClass($type, $idp)
  {
    return $this->clang->getProcessingClass($type, array( "idp" => $idp ));
  }

  public function computeInDB($params)
  {
    $elts = $this->getEntries();
    foreach($elts as $v)
      {
	$proc = $this->clang->getProcessingClass($v["wt_type"], 
						 array("idp" => $v["id"]));	
	$proc->computeInDB(array_merge( $params, array( "idp" => $v["id"] ) ));
      }
  }

  public function getValuesInDB($params)
  {
    $data = array();
    $elts = $this->getEntries();
    foreach($elts as $v)
      {
	$proc = $this->clang->getProcessingClass($v["wt_type"], 
						 array("idp" => $v["id"]));	
	$data[$v["id"]] = $proc->getValuesInDB(array_merge( $params, array( "idp" => $v["id"] ) ));
      }    
    return $data;
  }

}

?>