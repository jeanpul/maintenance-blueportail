<?php

include_once("BluePHP/BTopLocalServer/Config.inc");
include_once("BluePHP/Utils/ModuleProcessing.inc");
include_once("BluePHP/BTopLocalServer/BlueCountLang.inc");

class BTopSensors extends ModuleProcessing
{
  var $clang = false;

  public function __construct($sharedCon = null)
  {
    parent::__construct(array( "name" => "CountersAvailable",
			       "keyname" => "id",
			       "sort" => "id" ),
			$sharedCon);
    $this->clang = new BlueCountLang();
  }

  function getCountersAvailable()
  {
    return $this->Select("clientId from CountersAvailable group by clientId");
  }

  function getEntries()
  {
    return $this->Select("C.id AS id, ref, clientId, channel, " . 
			 "C.counter AS counter, host, descr, Flows.name AS door, " . 
			 "CASE WHEN CountersAssoc.sens=1 THEN L1.name ELSE L2.name END AS nameL1," . 
			 "CASE WHEN CountersAssoc.sens=1 THEN L2.name ELSE L1.name END AS nameL2 " .
			 "FROM CountersAvailable as C, Locations as L1, Locations as L2, Flows, CountersAssoc " . 
			 "WHERE C.id = CountersAssoc.counter AND CountersAssoc.flow = Flows.id AND " .
			 "Flows.zone1 = L1.id AND Flows.zone2 = L2.id");
  }

  function getEntry($params)
  {
    if(!isset($params["id"]))
      {
	return array( "clientId" => false,
		      "channel" => false,
		      "counter" => false,
		      "host" => false,
		      "descr" => false,
		      "door" => false,
		      "sens" => false );
      }
    else
      {
	$data = $this->Select("C.id as id, ref, clientId, channel, " .
			      "C.counter as counter, host, descr, Flows.id as door, sens, " . 
			      "CASE WHEN CountersAssoc.sens=1 THEN L1.name ELSE L2.name END AS nameL1," . 
			      "CASE WHEN CountersAssoc.sens=1 THEN L2.name ELSE L1.name END AS nameL2 " .
			      "FROM CountersAvailable as C, Locations as L1, Locations as L2, Flows, CountersAssoc " . 
			      "WHERE C.id = " . $params["id"] . " AND C.id = CountersAssoc.counter AND CountersAssoc.flow = Flows.id AND " .
			      "Flows.zone1 = L1.id AND Flows.zone2 = L2.id");
	return $data[0];
      }
  }

  function getNumberOfSensors()
  {
    $res = 0;
    $data = $this->Select("count(*) as n from CountersAvailable");
    if(is_array($data) and count($data))
      {
	$res = $data[0]["n"];
      }
    return $res;
  }

  function getNumberOfSensorsGrab()
  {
    $res = 0;
    $data = $this->Select("count(*) as n from CountersAvailable,GrabIPTable where clientId=substr(GrabIPTable.ref,1,length(clientId)) and laststatus='OK'");
    if(is_array($data) and count($data))
      {
	$res = $data[0]["n"];
      }
    return $res;
  }

  function newEntry($params)
  {
    return $this->clang->processBTopSensorInsert($params);
  }

  function setEntry($params)
  {
    return $this->clang->processBTopSensorUpdate($params);
  }

}

?>