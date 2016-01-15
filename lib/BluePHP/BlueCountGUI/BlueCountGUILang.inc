<?php

include_once("BluePHP/BTopLocalServer/BlueCountLang.inc");
include_once("BluePHP/BlueCountGUI/ConfigDB.inc");

/**
 * \file BlueCountGUILang.inc
 * \brief Interface to BlueCountGUI data
 */

/**
 * \class BlueCountGUILang
 * \brief Interface to BlueCountGUI data
 *
 * Use the configuration file to retrieve the
 * BlueCountGUI data parameters
 */
/** \addtogroup BlueCountLang *
/** @{ */
class BlueCountGUILang 
{
  var $config;

  var $clang;

  function BlueCountGUILang()
    {
      $this->clang = new BlueCountLang();
      $this->config = new ConfigDB();
    }

  /**
   * \brief call any method of the BlueCountGUILang class through its symbolic name
   *
   * \param $funcname the method symbolic name
   * \param $params the method parameters contained into an array indexed by the parameter name
   * \return the array list returned by the method
   */
  function callFunction($funcname, $params)
    {
      return call_user_func(array($this, $funcname), $params);
    }

  /**
   * \brief Returns the shared data
   */
  function getSharedConfigData($params = "")
    {
      return $this->config->getSharedConfigData();
    }

  /**
   * \brief Returns the user data
   *
   * \params 
   * - login [STRING]
   * - access [INT]
   * - configSection [STRING]
   * - userSection [STRING]
   */
  function getConfigData($params)
    {
      return $this->config->getConfigData(array( "login" => $params["login"], "access" => $params["access"] ), $params["configSection"], $params["userSection"]);
    }

  /**
   * \brief Returns the list of entities with their respective parameters.
   *
   * \param $params the array parameters wich contains :
   * - Analysis [STRING]
   * - Entity type ('location' | 'door' | 'groop' | 'area' ) : the type of wanted Entity
   * - login [STRING]
   * - access [INT]
   * - includeAll [true|false] : include not enabled entities too
   */
  function getCountersIds($params) 
    {
      global $Analysis;

      $res = array();

      // get the counters from BlueCountLang
      // first search for entities related to an analysis
      if(isset($params["Analysis"]) and isset($params["rfunc"]))
	{
	  $rfunc = $params["rfunc"];
	}
      else if($params["Entity"] == "location")
	{ 
	  $table = "Zones";
	  $rfunc = "getLocations";
	}
      else if($params["Entity"] == "area")
	{
	  $table = "Zones";
	  $rfunc = "getLocationAreas";
	}
      else if($params["Entity"] == "door")
	{	      
	  $table = "Flows";
	  $rfunc = "getDoors";
	}
      else if($params["Entity"] == "group")
	{
	  $table = "Flows";
	  $rfunc = "getDoorGroups";
	}

      $counters = $this->clang->$rfunc($params);

      if(!$counters or !is_array($counters) or !count($counters))
	{
	  return $res;
	}

      if($params["Entity"] == "location" or $params["Entity"] == "area")
	{
	  $table = "Zones";
	}
      else
	{
	  $table = "Flows";
	}

      $eName = $this->config->getConfigData(array( "login" => $params["login"], 
						   "access" => $params["access"] ), 
					    $table . "Name");
      $eEnabled = $this->config->getConfigData(array( "login" => $params["login"], 
						      "access" => $params["access"] ), 
					       $table . "Enabled");
      
      foreach($counters as $k => $v)
	{
	  // $k is the id
	  // $v is an array composed of ["idp"], ["id"], ["name"], ["virtual"]
	  if(!isset($eEnabled[$k]))
	    {
	      // if the entry does not exist
	      // create it with enabled default value and access granted for all
	      $this->config->addSharedConfigData($k, "true", 0, $table . "Enabled");
	      $eEnabled[$k]["value"] = true;
	    }

	  if(!isset($eName[$k]))
	    {
	      // if the user name does not exist
	      // create it with the BTopLocalServer default value
	      $this->config->addSharedConfigData($k, $v["name"], 0, $table . "Name");
	      $eName[$k]["value"] = $v["name"];
	    }

	  $v["enabled"] = $eEnabled[$k]["value"] == "true";
	  $v["oname"] = $v["name"];
	  $v["name"] = $eName[$k]["value"];

	  if($v["enabled"] or (isset($params["includeAll"]) and $params["includeAll"]))
	    {
	      // make this entity enabled
	      $res[$k] = $v;	 
	    }
	}
      return $res;
    }

  function getEntitiesFromAnalysis($params)
    {
      global $Analysis;

      $entities = $Analysis[$params["Analysis"]]["entity"];

      $elts = array();
      foreach($entities as $k => $v)
	{
	  $elts[$k] = array( "id" => $k, "name" => $v );
	}

      return $elts;
    }

  function getTimeZone()
    {
      $tz = $this->clang->getTimeZoneData( array() );
      return $tz[0]["tz"];
    }
  

  function getIndicatorParams($params)
  {
    global $AnalysisParams;

    $basepath = BASEPATH . "BlueCountGUI/indicators/";
    $filename = sprintf($AnalysisParams[$params["Analysis"]], $params["idp"]);    

    if(!file_exists($basepath . $filename))
      {
	$filename = sprintf($AnalysisParams[$params["Analysis"]], "default"); 
      }
    include_once($basepath . $filename);
    return getIndicatorParams($params);
  }

  //  function getIndicatorParameter

  function close()
    {
      $this->clang->close();
      $this->config->close();
    }
};

?>
