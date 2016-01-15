<?php

include_once("BluePHP/BTopLocalServer/Config.inc");
include_once("BluePHP/Utils/ModuleProcessing.inc");
include_once("BluePHP/BTopLocalServer/FlowCountingProcessing.inc");

class Flows extends ModuleProcessing
{
  public function __construct($sharedCon = null)
  {
    parent::__construct(array( "name" => "Flows",
			       "keyname" => "id",
			       "sort" => "id" ),
			$sharedCon);
  }

  function newEntry($params)
  {
    // get a new door id
    $res = $this->Select("ifnull(max(id) + 1, 0) as id from Flows");
    if(count($res))
      {
	$params["id"] = $res[0]["id"];

	// creates the new door
	$query = "INSERT INTO Flows(id, name, zone1, zone2) VALUES(" . 
	  $params["id"] .  ",\"" . $params["name"] . "\", " . $params["zone1"] . 
	  ", " . $params["zone2"] . " );";
	$this->QueryNoReturn($query);

	$module = new FlowCountingProcessing($this);
	$module->newEntry(array( "flow" => $params["id"] ));
      }
    else
      {
	$params["id"] = null;
      }
    return $params;
  }

  function setEntry($params)
  {
    $query = "UPDATE Flows SET name=\"" . $params["name"] . "\"" .
      ", zone1=" . $params["zone1"] . ", zone2=" . $params["zone2"] .
      " WHERE id=" . $params["id"];
    $this->QueryNoReturn($query);
    return $params;
  }

  function getOrientedEntry($params)
  {
    return $this->Select("Flows.id as id,Flows.name as name, 1 as sens, zone1, zone2, Z1.name as nameZ1," .
			 "Z2.name as nameZ2 from Flows, Zones as Z1,Zones as Z2 where Flows.id=" . $params["id"] . 
			 " and zone1=Z1.id and zone2=Z2.id union " . 
			 " select Flows.id as id, Flows.name as name, 2 as sens, zone1, zone2, Z1.name as nameZ1," .
			 "Z2.name as nameZ2 from Flows, Zones as Z1, Zones as Z2 where Flows.id=" . $params["id"] . 
			 " and zone1=Z2.id and zone2=Z1.id");
  }
}

?>