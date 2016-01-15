<?php

include_once("BluePHP/BTopLocalServer/Config.inc");
include_once("BluePHP/Utils/ModuleProcessing.inc");
include_once("BluePHP/BTopLocalServer/Flows.inc");

class Doors extends Flows
{
  public function __construct($sharedCon = null)
  {
    parent::__construct($sharedCon);
  }

  function getEntries()
  {
    return $this->Select("Doors.id as id,Doors.name as name, 1 as sens, zone1, zone2, Z1.name as nameZ1," .
			 "Z2.name as nameZ2 from Doors,Zones as Z1,Zones as Z2 where zone1=Z1.id " . 
			 "and zone2=Z2.id");
  }

  function getEntry($params)
  {
    $query = "Doors.id as id,Doors.name as name, 1 as sens, zone1, zone2, Z1.name as nameZ1," .
      "Z2.name as nameZ2 from Doors, Zones as Z1,Zones as Z2 where Doors.id=" . $params["id"] . 
      " and zone1=Z1.id and zone2=Z2.id";
    $elts = $this->Select($query);
    return $elts[0];
  }

}

?>