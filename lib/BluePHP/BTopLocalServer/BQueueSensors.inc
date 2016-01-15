<?php

include_once("BluePHP/Utils/ModuleProcessing.inc");
include_once("BluePHP/BTopLocalServer/Config.inc");
include_once("BlueCountLang.inc");

class BQueueSensors extends ModuleProcessing
{
  var $clang = false;

  public function __construct($sharedCon = null)
  {
    parent::__construct( array( "name" => "NumberingAvailable",
				"keyname" => "id",
				"sort" => "id" ),
			 $sharedCon);
    $this->clang = new BlueCountLang();
  }

  function getLocations()
  {
    return $this->clang->getLocations();
  }

  function getEntries()
  {
    return $this->Select("N.id as id, ref, clientId, channel, region, host, descr, " . 
			 "Locations.name as location from NumberingAvailable as N, " . 
			 "Locations, NumberingAssoc where " .
			 "N.id = NumberingAssoc.numbering and " . 
			 "Locations.id = NumberingAssoc.zone");
  }

  function getEntry($params)
  {
    if(!isset($params["id"]))
      {
	return array( "clientId" => false,
		      "channel" => false,
		      "region" => false,
		      "host" => false,
		      "descr" => false,
		      "location" => false );
      }
    else
      {
	$data = $this->Select("id, clientId, channel, region, host, descr, ref, " . 
			      "NumberingAssoc.zone as location from NumberingAvailable, " . 
			      "NumberingAssoc where id=" . $params["id"] .
			      " and NumberingAssoc.numbering=" . $params["id"]);
	return $data[0];
      }
  }

  function newEntry($params)
  {
    return $this->clang->processBQueueSensorInsert($params);
  }

  function setEntry($params)
  {
    return $this->clang->processBQueueSensorUpdate($params);
  }

  function delEntry($params)
  {
    return $this->clang->processBQueueSensorDelete($params);
  }
}

?>
