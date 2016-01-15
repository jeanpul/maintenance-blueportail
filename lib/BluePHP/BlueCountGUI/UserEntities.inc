<?php 

include_once("BluePHP/BlueCountGUI/BlueCountGUILang.inc");
include_once("BluePHP/BlueCountGUI/ConfigDB.inc");

class UserEntities
{
  var $glang;
  var $user;

  var $locations;
  var $areas;
  var $doors;
  var $groups;

  function UserEntities($glang, $params)
    {
      $this->glang = $glang;
      $this->user = $params;

      $this->getEntities();
    }

  function getEntities()
    {
      $this->locations = $this->get_entities("getLocations", "Zones");
      $this->areas = $this->get_entities("getLocationAreas", "Zones");
      $this->doors = $this->get_entities("getDoors", "Flows");
      $this->groups = $this->get_entities("getDoorGroups", "Flows");
    }

  function get_entities($rfunc, $table)
    {
      $data = $this->glang->clang->$rfunc($this->user);

      if(!$data or !is_array($data) or !count($data))
	{
	  return array();
	}
     
      $eName = $this->glang->config->getConfigData($this->user, $table . "Name");
      $eEnabled = $this->glang->config->getConfigData($this->user, $table . "Enabled");
      
      $res = array();
      foreach($data as $k => $v)
	{
	  // automatically create an user entry if the entity does not exist
	  //@{
	  if(!isset($eEnabled[$k]))
	    {
	      $this->glang->config->addSharedConfigData($k, "true", 0, $table . "Enabled");
	      $eEnabled[$k]["value"] = true;
	    }

	  if(!isset($eName[$k]))
	    {
	      $this->glang->config->addSharedConfigData($k, $v["name"], 0, $table . "Name");
	      $eName[$k]["value"] = $v["name"];
	    }
	  //@}

	  $v["enabled"] = $eEnabled[$k]["value"] == "true";
	  $v["oname"] = $v["name"];
	  $v["name"] = $eName[$k]["value"];

	  $res[$k] = $v;
	}
      return $res;
    }

  function getLocationName($id)
    {
      return $this->locations[$id]["name"];
    }

  function getAreaName($id)
    {
      return $this->areas[$id]["name"];
    }

  function getDoorName($id)
    {
      return $this->doors[$id]["name"];
    }

  function getGroupName($id)
    {
      return $this->groups[$id]["name"];
    }  
}

?>