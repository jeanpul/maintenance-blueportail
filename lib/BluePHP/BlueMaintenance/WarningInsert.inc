<?php

include_once("BluePHP/Utils/ModuleProcessing.inc");
include_once("BluePHP/BlueProjectPHP/Projects.inc");
include_once("BluePHP/Utils/DBConnect.inc");
include_once("BluePHP/BlueMaintenance/Config.inc");

class WarningInsert extends ModuleProcessing {

  public function __construct() {    
    parent::__construct(array("name" => "AdvancedWarning",
	"keyname" => array("ID", "AttributeName")));

    $this->setConParams(array("type" => "SQLITE3",
	"host" => "localhost",
	"name" => BLUEMAINTENANCEDBNAME,
	"user" => "blue",
	"passwd" => "blue"
    ));
  }
  
  public function setEntry($params) {    
    parent::setEntry($params);        
  }
  
  public function LastEntry($id,$attributeName) {    
    $res = $this->Select("* from AdvancedWarning where ID=\"$id\" and AttributeName=\"$attributeName\" and Terminated=\"0\" order by DateEnd desc limit 1");   
    return (is_array($res) and count($res)) ? $res[0] : null;
  }
  

  function updateWarning($id, $ref, $dateEnd, $attributeName, $terminated = 0) {
    $res = $this->LastEntry($id,$attributeName);
    $first = $this->Select("* from AdvancedWarning where ID=\"$id\" and AttributeName=\"$attributeName\"and Terminated=\"0\"");
    if ($first == null) {
      $this->newEntry(array_merge(array("ID" => $id), array("Ref" => $ref), array("AttributeName" => $attributeName), array("DateStart" => $dateEnd), array("DateEnd" => $dateEnd), array("Terminated" => $terminated)));
    } else {
      $this->setEntry(array_merge(array("ID" => $id), array("Ref" => $ref), array("AttributeName" => $attributeName), array("DateStart" => $res["DateStart"]), array("DateEnd" => $dateEnd, array("Terminated" => $terminated))));
    }
  }
  
  public function WhatState($id, $date){
    $res = $this->Select("* from AdvancedWarning where ID=\"$id\" and DateEnd=\"$date\" order by DateStart desc limit 1");
    if ($res != null) {
      $state = "Warning";
    }else {
      $state = "OK";
      $this->QueryNoReturn("update AdvancedWarning set Terminated=\"1\" where ID=\"$id\" and Terminated=\"0\"");
    }
    return $state;
  }
  
  private function generateQuery($fields, $params, $extra) {
    $query = "$fields from " . $this->getName();
    $firstElem = true;
    $sep = " where ";
    foreach ($params as $k => $v) {
      if (!is_null($v)) {
	$query .= $sep . $k . "='" . $v . "'";;
	if ($firstElem) {
	  $sep = " and ";
	  $firstElem = false;
	}
      } 
    }
    return $query . " " . $extra;
  }
  
  private function convertTimezone($date, $timezone) {    
    $datetime = new DateTime($date);      
    $datetime->setTimezone(new DateTimeZone($timezone));
    return $datetime->format('Y-m-d H:i:s');
  }
  
  private function createDataWithProjectInfo($data, $timezone, $useTimezone = false) {   
    $projectsInfo = array();   
    $p = new Projects();    
    $rows = array();
    foreach($data as $obj) {		        
      if($useTimezone) {
	$obj["DateStart"] = $this->convertTimezone($obj["DateStart"], $timezone);
	$obj["DateEnd"] = $this->convertTimezone($obj["DateEnd"], $timezone);
      }
      $rows[] = array_values($obj); 
     
    } 
return $rows; 
  }

  protected function SelectRowWarningHistoricGeneric($params,$projectNumber,$useTimezone = false) {
    $p = new Projects(); 
    $query = $this->generateQuery("AttributeName,DateStart,DateEnd,ID,Ref", $params, "");
    $data = $this->Select($query);   
    $res= $p->Select("timezone from projects where project_id=\"" . $projectNumber . "\""); 
    return $this->createDataWithProjectInfo($data, $res[0]['timezone'], $useTimezone);
  }
}

?> 