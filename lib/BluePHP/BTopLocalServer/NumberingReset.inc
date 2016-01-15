<?php

include_once("BluePHP/BTopLocalServer/Config.inc");
include_once("BluePHP/Utils/DBConnect.inc");
include_once("BluePHP/Utils/DateOps.inc");
include_once("BluePHP/BTopLocalServer/GlobalProperties.inc");

class NumberingReset
{
  private $tableCreation = "create table if not exists NumberingReset (id INTEGER, start TIMESTAMP, value INTEGER, PRIMARY KEY(start, id))";
  private $con = null;

  function __construct()
  {
  }

  function getEntry($params)
  {
    return array();
  }

  function setEntry($params)    
  {
    if(!isset($params["start"]))
      {
	$module = new GlobalProperties();
	$params["TimeStr"] = $module->getLastProcessedTimeString();
	$params["start"] = convertToDBTimeFormat($params["TimeStr"]);
      }
    else
      {
	$params["TimeStr"] = convertCommonTimeString($params["start"]);
      }
    return $this->setValue($params);
  }

  function getValues($params)
  {
    $res = array();
    if($this->connectToDB($params))
      {
	// now retrieve reset values
	$query = "select * from NumberingReset where id=" . $params["id"] . " order by start asc";
	$result = $this->con->query($query);
	if($result)
	  {
	    while($obj = $result->nextAssoc())
	      {
		$res[$obj["start"]] = $obj;
	      }
	    $result->freeResult();
	  }
      }
    return $res;
  }

  function setValue($params)
  {
    $res = false;
    if($this->connectToDB($params))
      {
	// now insert or update the value
	$query = "insert or replace into NumberingReset (id, start, value) values(" . 
	  $params["id"] . ",'" . $params["start"] . "'," . $params["value"] . ")";
	$result = $this->con->query($query);
	$res = $result != null;
      }
    return $params;
  }

  protected function connectToDB($params)
  {
    $res = false;
    $dbName = BTOP_SERVER_DBPATH . "/days/BCL_" . substr($params["TimeStr"], 0, 8) . ".db";
    if(file_exists($dbName))
      {
	$this->con = new DBConnect(CONFIGDBTYPE,
				   CONFIGDBHOST,
				   $dbName,
				   CONFIGDBUSER,
				   CONFIGDBPASSWD );
	$this->con->connectToDB();
	// force table creation if missing
	$result = $this->con->query($this->tableCreation);
	$result->freeResult();
	$res = true;
      }
    return $res;
  }

}

?>