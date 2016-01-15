<?php

include_once("BluePHP/BTopLocalServer/Config.inc");
include_once("BluePHP/Utils/ModuleProcessing.inc");
include_once("BluePHP/BTopLocalServer/Zones.inc");
include_once("BluePHP/BTopLocalServer/BlueCountLang.inc");

/**
 * Represents the Areas entities into
 * the BTopLocalServer
 */
class Areas extends Zones
{
  private $clang = null; /**< pointer to the db data manager */

  /**
   * Instanciate the db data manager
   */
  public function __construct($sharedCon = null)
  {
    parent::__construct($sharedCon);
    $this->clang = new BlueCountLang();
  }

  /**
   * Returns an array of all the areas entities
   */
  function getEntries()
  {
    return $this->Select("* from Zones where exists(select * from ZoneZoneAssoc as zza where " . 
			 "zza.zone=Zones.id)");
  }

  /**
   * Returns a detailled version of a specific area entity
   */
  function getEntry($params)
  {
    $query = "Zones1.id as id, Zones1.name as name, ZoneZoneAssoc.contained as idZ," .
      "Zones2.name as nameZ from Zones as Zones1, Zones as Zones2, ZoneZoneAssoc " .
      "where Zones1.id=" . $params["id"] . " and Zones1.id = ZoneZoneAssoc.zone and " .
      "Zones2.id=ZoneZoneAssoc.contained";
    $elts = $this->Select($query);
    if(is_array($elts) and count($elts))
      {
	$params = array( "id" => $params["id"],
			 "name" => $elts[0]["name"],
			 "zones" => array_map(function($v) { return $v["idZ"]; }, $elts));
      }
    return $params;
  }

  /**
   * \brief Changes the parameters values of the specified Area
   * 
   * Changes the parameters values of the specified Area
   * \param $params the parameters array which contains : 
   * - id (INT >= 0) : the unique id
   * - name (STRING) : the new name
   * - zones (ARRAY OF INT >= 0) : an array of all the new locations
   * zones could also be a string of comma delimited list of locations (z1,z2,z3,...)
   *   contained into this area
   */
  function setEntry($params)
  {
    parent::setEntry($params);

    // delete previous location components
    $query = "DELETE FROM ZoneZoneAssoc WHERE zone=" . $params["id"];
    $this->QueryNoReturn($query);

    if(!is_array($params["zones"]))
      {
	$params["zones"] = explode(",", $params["zones"]);
      }

    // insert new location components
    foreach($params["zones"] as $v)
      {
	$query = "INSERT INTO ZoneZoneAssoc(zone, contained) VALUES(" . $params["id"] . ", $v)";
	$this->QueryNoReturn($query);
      }
    return $params;
  }

  /**
   * Insert a new area entity
   */
  function newEntry($params)
  {
    if(is_array($params["zones"]))
      {
	$params["locations"] = $params["zones"];
      }
    else
      {
	$params["locations"] = explode(",", $params["zones"]);
      }
       
    return $this->clang->processAreaInsert($params);
  }
}

?>