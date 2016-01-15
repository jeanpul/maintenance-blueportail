<?php

include_once("BluePHP/Utils/ModuleProcessing.inc");
include_once("BluePHP/Utils/DBConnect.inc");
include_once("BluePHP/BlueProjectPHP/Projects.inc");
include_once("BluePHP/BlueMaintenance/Config.inc");
include_once("BluePHP/BlueMaintenance/WarningInsert.inc");

class BaseMaintenance extends ModuleProcessing {

  protected function __construct($params) {
    parent::__construct($params);
    $this->setConParams(array("type" => "SQLITE3",
			      "host" => "localhost",
			      "name" => BLUEMAINTENANCEDBNAME,
			      "user" => "blue",
			      "passwd" => "blue"
			      ));
  }

  public function getLastStateFromId($id) {
    $res = $this->Select("* from " . $this->getName() . " where ID=\"$id\" order by DateStart desc limit 1");
    return (is_array($res) and count($res)) ? $res[0] : null;
  }

  public function getLastEntriesNumeroP($numerop) {
    return $this->Select("* from " . $this->getName() . " where NumeroP=$numerop group by ID " .
			 "order by DateStart desc");
  }

  public function DeleteID($id) {
    $this->QueryNoReturn("delete from " . $this->getName() . " where ID=\"$id\"");
  }

  public function ChangeID($id, $newId) {
    $wobj = new WarningInsert();
    $wobj->QueryNoReturn("update AdvancedWarning set ID=\"$newId\" where ID=\"$id\"");
    $this->QueryNoReturn("update " . $this->getName() . " set ID=\"$newId\" where ID=\"$id\"");
  }
  
  public function RefreshID($id , $ref) {
    $res = $this->getLastStateFromId($id);
    exec(BLUEMAINTENANCEROOT . "/Config/".$res['NumeroP']."/connect");
    exec("/etc/BEV/scripts/BlueMaintenance/recupWeb.sh ".$res['Serveur']." ".$res['NumeroP']."");
    exec(BLUEMAINTENANCEROOT . "/Config/".$res['NumeroP']."/disconnect"); 
  }
  
  public function Check($id, $ref, $date = null) 
  {
    if(is_null($date))
      {
	$date = strftime("%Y-%m-%d %H:%M:%S");
      }

    $res = $this->getLastStateFromId($id);
    if($res)
      {
	if($res["Etat"] == "OK") 
	  {
	    $this->setEntry(array_merge($res, array("Etat" => "Checked", "DateEnd" => $date ) ));
	  } 
	else if ($res["Etat"] == "Warning") 
	  {
	    $this->setEntry(array_merge($res, array("Etat" => "WarningChecked", "DateEnd" => $date ) ));
	  } 
      }
    return $res ? $this->getLastStateFromId($id) : null;
  }

  public function UnCheck($id, $ref, $date = null) 
  {
    if(is_null($date))
      {
	$date = strftime("%Y-%m-%d %H:%M:%S");
      }

    $res = $this->getLastStateFromId($id);
    if($res)
      {
	if($res["Etat"] == "Checked") 
	  {
	    $this->setEntry(array_merge($res, array("Etat" => "OK", "DateEnd" => $date ) ));
	  } 
	else if ($res["Etat"] == "WarningChecked") 
	  {
	    $this->setEntry(array_merge($res, array("Etat" => "Warning", "DateEnd" => $date ) ));
	  } 
      }
    return $res ? $this->getLastStateFromId($id) : null;
  }
  
  private function convertTimezone($date, $timezone) {    
    $datetime = new DateTime($date);      
    $datetime->setTimezone(new DateTimeZone($timezone));
    return $datetime->format('Y-m-d H:i:s');
  }
  
  private function generateQuery($fields, $params, $extra) {
    $query = "$fields from " . $this->getName();
    $firstElem = true;
    foreach ($params as $k => $v) {
      if (!is_null($v)) {
	$sep = " where ";
	$query .= $sep . $k . "='" . $v . "'";
	if ($firstElem) {
	  $sep = " and ";
	  $firstElem = false;
	}		
      } 
    }
    return $query . " " . $extra;
  }

  private function createDataWithProjectInfo($data, $useTimezone = false) {   
    $projectsInfo = array();   
    $p = new Projects();    
    $rows = array();
    foreach($data as $obj) {
      if (!isset($projectsInfo[$obj["NumeroP"]])) {
	$res = $p->getEntry(array("project_id" => $obj["NumeroP"]));
	$projectsInfo[$obj["NumeroP"]] = $res;	
	
      }	        
      if($useTimezone) {
	$obj["DateStart"] = $this->convertTimezone($obj["DateStart"], $projectsInfo[$obj["NumeroP"]]["timezone"]);
	$obj["DateEnd"] = $this->convertTimezone($obj["DateEnd"], $projectsInfo[$obj["NumeroP"]]["timezone"]);
      }
      //$obj["ProjectName"] = $projectsInfo[$obj["NumeroP"]]["project_name"];
     array_unshift($obj,  $projectsInfo[$obj["NumeroP"]]["project_name"]);
     $rows[] = array_values($obj);     
    
}
    
    return $rows;
  }

  private function selectRowGeneric($fields, $params, $extra)
  {
    $query = $this->generateQuery($fields, $params, $extra);
    $data = $this->Select($query);      
    return $this->createDataWithProjectInfo($data);        
  }
  
  public function SelectRowLast($params) {    
    return $this->selectRowGeneric("Ref,ID,Etat,DateEnd,NumeroP", $params, "group by ID");
  }

  public function SelectRowAll($params) {
    return $this->selectRowGeneric("Ref,ID,Etat,DateEnd,NumeroP", $params, "");
  }

  public function SelectRowChecked($params) {
    return $this->selectRowGeneric("Ref,ID,Etat,DateEnd,NumeroP", $params, "group by ID Having Etat=\"OK\" or Etat=\"Warning\"");    
  }

  public function SelectRowWarning($params) {
    return $this->selectRowGeneric("Ref,ID,Etat,DateEnd,NumeroP", $params, "group by ID Having Etat=\"WarningChecked\" or Etat=\"Warning\"");    
  }

  public function SelectRowHistoric($params, $useTimezone = false) {
    $query = $this->generateQuery("Serveur,Ref,ID,Etat,DateStart,DateEnd,NumeroP", $params, "");
    $data = $this->Select($query);      
    return $this->createDataWithProjectInfo($data, $useTimezone);
    $wobj= new WarningInsert();
  }
  

}

?>
