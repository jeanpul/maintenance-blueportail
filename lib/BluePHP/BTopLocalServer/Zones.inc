<?php

include_once("BluePHP/BTopLocalServer/Config.inc");
include_once("BluePHP/Utils/ModuleProcessing.inc");
include_once("BluePHP/BTopLocalServer/NumberingProcessing.inc");
include_once("BluePHP/BTopLocalServer/ZoneCounting.inc");

class Zones extends ModuleProcessing
{
  public function __construct($sharedCon = null)
  {
    parent::__construct(array( "name" => "Zones",
			       "keyname" => "id",
			       "sort" => "id" ),
			$sharedCon);
  }

  function setEntry($params)
  {
    $this->QueryNoReturn("update Zones set name=\"" . $params["name"] . "\"" . 
			 (isset($params["virtual"]) ? ",virtual=" . $params["virtual"] : "") . 
			 " where id=" . $params["id"]);
    return $params;
  }

  function newEntry($params)
  {
    // get a new location id
    $res = $this->Select("ifnull(max(id) + 1, 0) as id from Zones");
    if(count($res))
      {
	$params["id"] = $res[0]["id"];

	// creates the new location
	$query = "INSERT INTO Zones(id, name, virtual) VALUES(" . 
	  $params["id"] .  ",\"" . $params["name"] . "\", " . 
	  (isset($params["virtual"]) ? $params["virtual"] : "0") . ")";
	$this->QueryNoReturn($query);

	$module = new ZoneCounting($this);
	$module->newEntry(array( "zone" => $params["id"]));

	$module = new NumberingProcessing($this);
	$module->newEntry(array( "zone" => $params["id"]));
      }
	
      return $params;
  }

}

?>