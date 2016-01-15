<?php

include_once("BluePHP/BTopLocalServer/Config.inc");
include_once("BluePHP/Utils/DateOps.inc");
include_once("BluePHP/BTopLocalServer/GlobalProperties.inc");
include_once("BluePHP/Utils/ModuleProcessing.inc");
include_once("BluePHP/BTopLocalServer/BlueCountLang.inc");

// Fab : TODO this class should inherit
// to a common class with FlowProc, ProcessTime,
// and WTProcess in order to share common operations
class NumberingProcessing extends ModuleProcessing
{
  private $clang = false;

  function __construct($sharedCon = null)
  {
    parent::__construct(array( "name" => "ZoneNumberingProcessing",
			       "keyname" => "id",
			       "sort" => "id" ),
			$sharedCon);
    $this->clang = new BlueCountLang();
  }

  function getLastPushTask()
  {
    return $this->clang->getLastPushTask();
  }

  function getEntries()
  {
    return $this->clang->getZoneNumberingProcessing();
  }

  function getZones()
  {
    return $this->clang->getLocationsWithNumberingProcessing();
  }

  function getAllZones()
  {
    return $this->clang->getZones(array("type" => 0));
  }

  function getCalendars()
  {
    return parent::Select("* from Calendars");
  }

  function getFunctions()
  {
    return parent::Select("* from NumberingType");
  }

  public function haveType($typeName)
  {
    $data = parent::Select("count(*) as c from ZoneNumberingProcessing, NumberingType where " . 
			   "numb_id=NumberingType.id and name='$typeName'");
    return (int) $data[0]["c"];
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
    return $this->clang->getNumberingValues($params);
  }

  function getNumberingValues($params)
  {
    if(!isset($params["idp"]) and isset($params["id"]))
      {
	$params["idp"] = $params["id"];
      }
    return $this->clang->getNumberingValues($params);
  }

  function getValues($params)
  {
    if(isset($params["id"])) 
      {
	$params["idp"] = $params["id"];
      }
    $params["valueKind"] = "numbering";
    $params["Entity"] = "area";
    $data = $this->clang->getValues($params);
    return $data;
  }

  public function getProcessingClass($type, $idp)
  {
    return $this->clang->getProcessingClass($type, array( "idp" => $idp ));
  }

  function getAreaChildNumberingProcessing($params)
  {
    return $this->clang->getAreaChildNumberingProcessing($params);    
  }

  public function computeInDB($params)
  {
    $elts = $this->getEntries();
    foreach($elts as $v)
      {
	$proc = $this->clang->getProcessingClass($v["numb_type"], 
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
	$proc = $this->clang->getProcessingClass($v["numb_type"], 
						 array("idp" => $v["id"]));	
	$data[$v["id"]] = $proc->getValuesInDB(array_merge( $params, array( "idp" => $v["id"] ) ));
      }    
    return $data;
  }
}

?>