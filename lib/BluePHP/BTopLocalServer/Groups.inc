<?php

include_once("BluePHP/BTopLocalServer/Config.inc");
include_once("BluePHP/Utils/ModuleProcessing.inc");
include_once("BluePHP/BTopLocalServer/Flows.inc");

class Groups extends Flows
{
  public function __construct($sharedCon = null)
  {
    parent::__construct($sharedCon);
  }

  function getEntries()
  {
    $query = "Flows.id as id,Flows.name as name,zone1,zone2,Z1.name as nameZ1," .
      "Z2.name as nameZ2 from Flows,Zones as Z1," .
      "Zones as Z2 where zone1=Z1.id and zone2=Z2.id and " .
      "exists(select * from FlowFlowAssoc where flow=Flows.id)";
    return $this->Select($query);
  }

  function newEntry($params)
  {
    $params = parent::newEntry($params);
    if(!is_null($params["id"]))
      {
	$this->groupInsert($params);
      }
    return $params;
  }

  function setEntry($params)
  {
    $params = parent::setEntry($params);
    if(!is_null($params["id"]))
      {
	$query = "delete from FlowFlowAssoc where flow=" . $params["id"];
	$this->QueryNoReturn($query);
	$this->groupInsert($params);
      }
    return $params;
  }

  /**
   * \brief Insert the door in the door group in the FlowFlowAssoc table
   *
   * \param $params parameter array which contains
   * - id (INT >= 0) : the unique group id
   * - zone1 (INT >= 0) : the first zone id
   * - zone2 (INT >= 0) : the second zone id
   */
  protected function groupInsert($params)
  {
    // add the door components
    $query = "id, name, 1 as sens FROM Flows WHERE  id != " . $params["id"] . " AND " .
      "zone1 IN (SELECT contained FROM ZoneZoneResolvedAssoc WHERE zone=" . $params["zone1"] . ") AND " .
      "zone2 IN (SELECT contained FROM ZoneZoneResolvedAssoc WHERE zone=" . $params["zone2"] . ") AND " .
      "zone2 NOT IN (SELECT contained FROM ZoneZoneResolvedAssoc WHERE zone=" . $params["zone1"] . ") AND " .
      "zone1 NOT IN (SELECT contained FROM ZoneZoneResolvedAssoc WHERE zone=" . $params["zone2"] . ") AND " .
      "NOT EXISTS(SELECT * FROM FlowFlowAssoc WHERE flow = Flows.id) union " . 
      "SELECT id, name, 2 as sens FROM Flows WHERE  id != " . $params["id"] . " AND " .
      "zone2 IN (SELECT contained FROM ZoneZoneResolvedAssoc WHERE zone=" . $params["zone1"] . ") AND " .
      "zone1 IN (SELECT contained FROM ZoneZoneResolvedAssoc WHERE zone=" . $params["zone2"] . ") AND " .
      "zone1 NOT IN (SELECT contained FROM ZoneZoneResolvedAssoc WHERE zone=" . $params["zone1"] . ") AND " .
      "zone2 NOT IN (SELECT contained FROM ZoneZoneResolvedAssoc WHERE zone=" . $params["zone2"] . ") AND " .
      "NOT EXISTS(SELECT * FROM FlowFlowAssoc WHERE flow = Flows.id)";

    $elts = $this->Select($query);

    foreach($elts as $obj)
      {
	$query = "INSERT INTO FlowFlowAssoc(flow, contained, sens) VALUES(" . 
	  $params["id"] . "," . $obj["id"] . "," . $obj["sens"]. ")";
	$this->QueryNoReturn($query);
      }
  }
}

?>