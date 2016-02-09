<?php

include_once("BluePHP/Utils/DBConnect.inc");
include_once("BluePHP/Utils/DateOps.inc");

/**
 * \file BlueCountLang.inc
 * \brief Interface to BlueCount data
 */

/**
 * \class BlueCountLang
 * \brief Interface to BlueCount data
 *
 * Use the configuration file to retrieve
 * the BlueCount data server parameters
 */
/** \addtogroup BlueCountLang */
/** @{ */
class BlueCountLang
{
  /**
   * \private countDB
   * Connection to the raw counting server
   */
  var $countDB;

  /**
   * \private configDB
   * Connection to the config server
   */
  var $configDB;
  /**
   * \private dataDB
   * Connection to the data server
   */
  var $dataDB;

  /**
   * \brief Create the BlueCountLang interface
   */
  function BlueCountLang($userCountDB = false, $paramsDB = false)
  {
    if(!$paramsDB)
      {
	$this->openDBOld($userCountDB);
      }
    else
      {
	$this->openDB($paramsDB);
      }
  }

  function openDBOld($useCountDB = false)
    {
      if($useCountDB)
	{
	  $this->countDB = new DBConnect(COUNTDBTYPE, COUNTDBHOST, COUNTDBNAME,
					 COUNTDBUSER, CONFIGDBPASSWD);
	  $this->countDB->connectToDB();
	}
      else
	{
	  $this->countDB = false;
	}

      $this->configDB = new DBConnect(CONFIGDBTYPE, CONFIGDBHOST, CONFIGDBNAME,
				      CONFIGDBUSER, CONFIGDBPASSWD);
      $this->configDB->connectToDB();

      $this->dataDB = 0;
      /*$this->dataDB = new DBConnect(DATADBTYPE, DATADBHOST, DATADBNAME,
				      DATADBUSER, DATADBPASSWD);
      $this->dataDB->connectToDB();
      */
    }

  function openDB($params)
  {
    $this->configDB = new DBConnect($params["TYPE"], $params["HOST"], $params["NAME"], $params["USER"], $params["PASSWD"]);
    $this->configDB->connectToDB();
    $this->countDB = false;
    $this->dataDB = 0;
  }

  /**
   * \brief open the database containing the data matching the need
   *
   * \param $step the period to compute minute, hour, day ...
   * \param $timestamp the data for the computation
   */
  function accessDataDB($step, $timestamp)
    {
      if($this->dataDB)
	{
	  $this->dataDB->closeDB();
	  $this->dataDB = 0;
	}

      $dbname=BTOP_SERVER_DBPATH; // DATADBPATH;
      if($step == "day" || $step == "month" || $step == "year")
	{
	  $dbname .= "/BCL.db";
	}
      else
	{
	  $dbname .= "/days/BCL_" . strftime("%Y%m%d", $timestamp) . ".db";	  
	}

      if(file_exists($dbname))
	{
	  $this->dataDB = new DBConnect(DATADBTYPE, DATADBHOST, $dbname,
					DATADBUSER, DATADBPASSWD);
	  $this->dataDB->connectToDB();
	  return $this->dataDB;
	}
      else return false;
    }

  function queryDataDB($query)
  {
    $result = $this->dataDB->query($query);
    $data = $this->get_associative_array($result);
    $result->freeResult();
    return $data;
  }

  /**
   * \brief call any method of the BlueCountLang class through its symbolic name
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
   * \brief Clear all data stored 
   */
  function clearAll($params = "")
    {
      exec("/etc/BEV/BlueCount/default/createAllTables.sh " . BTOP_SERVER_DBPATH);
    }

  /**
   * \brief Returns the TimeStamp YYYYMMDDHHMMSS
   * of the current process
   */
  function getLastPushTask()
  {
    $query = "select value from GlobalProperties where name='lastPushTask'";
    $result = $this->configDB->query($query);
    $data = $result->nextAssoc();
    $result->freeResult();
    return $data["value"];
  }

  function getLastProcessTime()
  {
    $query = "select value from GlobalProperties where name='lastProcessTask'";
    $result = $this->configDB->query($query);
    $data = $result->nextAssoc();
    $result->freeResult();
    return $data["value"];
  }

  /**
   */
  function updateLastPushTask($timestr)
  {
    $query = "update GlobalProperties set value='$timestr' where name='lastPushTask'";
    $result = $this->configDB->query($query);
    $result->freeResult();
  }


  /**
   * \brief Returns raw counter database information
   *
   * Returns a list of values from the counter database :
   * - NbEntries (INT >= 0) : total number of counters entries
   * - SumValues (INT >= 0) : total sum of counters entries values
   * - MinTime (DATE) : Min Counter value timestamp for all the entries in the format YYYYMMDDHHMMSS
   * - MaxTime (DATE) : Max Counter value timestamp for all the entries in the format YYYYMMDDHHMMSS
   * - NbProcessed (INT >= 0) : total number of entries processed by the LocalServer
   */
  function getBTopCountingInfos($params = "")
    {
      $query = "select count(*) as NbEntries, sum(value) as SumValues, min(time) as MinTime, max(time) as MaxTime, sum(state) as NbProcessed from counting";
      $result = $this->countDB->query($query);
      $data = $result->nextAssoc();
      $result->freeResult();
      $data["NbProcessed"] = "_,_";
      return $data;
    }

  /**
   * \brief Returns raw numbering database information
   *
   * Returns a list of values from the numbering database :
   * - NbEntries (INT >= 0) : total number of numbering entries
   * - SumValues (INT >= 0) : total sum of numbering entries values
   * - MinTime (DATE) : Min Numbering value timestamp for all the entries in the format YYYYMMDDHHMMSS
   * - MaxTime (DATE) : Max Numbering value timestamp for all the entries in the format YYYYMMDDHHMMSS
   * - NbProcessed (INT >= 0) : total number of entries processed by the LocalServer
   */
  function getBQueueNumberingInfos($params = "")
    {
      $query = "select count(*) as NbEntries, sum(value) as SumValues, min(time) as MinTime, max(time) as MaxTime, sum(state) as NbProcessed from numbering";
      $result = $this->countDB->query($query);
      $data = $result->nextAssoc();
      $result->freeResult();
      $data["NbProcessed"] = "_";
      return $data;
    }

  /**
   * \brief Returns Location counting information 
   *
   * Returns a list of values from the location counting database :
   * - NbEntries (INT >= 0) : total number of counters entries
   * - SumValues0 (INT >= 0) : total sum of counters entries values
   * - SumValues1 (INT >= 0) : total sum of counters entries values
   * - MinTime (DATE) : Min Counter value timestamp for all the entries in the format YYYYMMDDHHMMSS
   * - MaxTime (DATE) : Max Counter value timestamp for all the entries in the format YYYYMMDDHHMMSS
   * - SumExpected0 (INT >= 0) : total number of entries expected by the LocalServer
   * - SumExpected1 (INT >= 0) : total number of entries expected by the LocalServer
   * - SumCumul0 (INT >= 0) : total number of entries processed by the LocalServer
   * - SumCumul1 (INT >= 0) : total number of entries processed by the LocalServer
   */
  function getLocationCountingInfos($params = "")
    {      
      if(!isset($params["timestamp"]))
	{
	  $params["timestamp"] = strftime("%s");
	}     
      if($this->accessDataDB("minute", $params["timestamp"]))    
	{
	  $params["type"] = 1;
	  $zoneprocessing = $this->getZoneCountingProcessing($params);
	  $idlist = "";
	  foreach($zoneprocessing as $z)
	    {
	      if($idlist != "") $idlist .= ", ";
	      $idlist .= $z["id"];
	    }
	  
	  $query = "select count(*) as NbEntries, sum(value0) as SumValues0, sum(value1) as SumValues1, min(start) as MinTime, "
	    . "max(start) as MaxTime, sum(nbexpected0) as SumExpected0, sum(nbexpected1) as SumExpected1, "
	    . "sum(nbcumul0) as SumCumul0, sum(nbcumul1) as SumCumul1 from ZoneCounting_MINUTE "
	    . "WHERE ZoneCounting_MINUTE.id IN (" . $idlist . ")";
	 
	  $result = $this->dataDB->query($query);
	  $data = $result->nextAssoc();
	  $result->freeResult();	
	  return $data;
	}
      else return array();
    }

  /**
   * \brief Returns Location numbering information 
   *
   * Returns a list of values from the location numbering database :
   * - NbEntries (INT >= 0) : total number of numbering entries
   * - SumValues (INT >= 0) : total sum of numbering entries values
   * - MinTime (DATE) : Min Numbering value timestamp for all the entries in the format YYYYMMDDHHMMSS
   * - MaxTime (DATE) : Max Numbering value timestamp for all the entries in the format YYYYMMDDHHMMSS
   * - SumExpected (INT >= 0) : total number of entries expected by the processing
   * - SumCumul (INT >= 0) : total number of entries processed by the LocalServer
   */
  function getLocationNumberingInfos($params = "")
    {      
      if(!isset($params["timestamp"]))
	{
	  $params["timestamp"] = strftime("%s");
	}     

      if($this->accessDataDB("minute", $params["timestamp"]))    
	{
	  $params["type"] = 1;
	  $zoneprocessing = $this->getZoneNumberingProcessing($params);
	  $idlist = "";
	  foreach($zoneprocessing as $z)
	    {
	      if($idlist != "") $idlist .= ", ";
	      $idlist .= $z["id"];
	    }
	  
	  $query = "select count(*) as NbEntries, sum(value) as SumValues, min(start) as MinTime, "
	    . "max(start) as MaxTime, sum(nbexpected) as SumExpected, sum(nbcumul) AS SumCumul from ZoneNumbering_MINUTE "
	    . "WHERE ZoneNumbering_MINUTE.id IN (" . $idlist . ")";	

	  $result = $this->dataDB->query($query);

	  $data = $result->nextAssoc();
	  $result->freeResult();	
	  return $data;
	}
      else return array();
    }

  /**
   * \brief Returns Area counting information 
   *
   * Returns a list of values from the location counting database :
   * - NbEntries (INT >= 0) : total number of counters entries
   * - SumValues0 (INT >= 0) : total sum of counters entries values
   * - SumValues1 (INT >= 0) : total sum of counters entries values
   * - MinTime (DATE) : Min Counter value timestamp for all the entries in the format YYYYMMDDHHMMSS
   * - MaxTime (DATE) : Max Counter value timestamp for all the entries in the format YYYYMMDDHHMMSS
   * - SumExpected0 (INT >= 0) : total number of entries expected by the LocalServer
   * - SumExpected1 (INT >= 0) : total number of entries expected by the LocalServer
   * - SumCumul0 (INT >= 0) : total number of entries processed by the LocalServer
   * - SumCumul1 (INT >= 0) : total number of entries processed by the LocalServer
   */
  function getAreaCountingInfos($params = "")
    {
      if(!isset($params["timestamp"]))
	{
	  $params["timestamp"] = strftime("%s");
	}     
     
      if($this->accessDataDB("minute", $params["timestamp"]))    
	{
	  $params["type"] = 2;
	  $zoneprocessing = $this->getZoneCountingProcessing($params);
	  $idlist = "";
	  foreach($zoneprocessing as $z)
	    {
	      if($idlist != "") $idlist .= ", ";
	      $idlist .= $z["id"];
	    }
	  
	  $query = "select count(*) as NbEntries, sum(value0) as SumValues0, sum(value1) as SumValues1, min(start) as MinTime, "
	    . "max(start) as MaxTime, sum(nbexpected0) as SumExpected0, sum(nbexpected1) as SumExpected1, "
	    . "sum(nbcumul0) as SumCumul0, sum(nbcumul1) as SumCumul1 from ZoneCounting_MINUTE "
	    . " WHERE id IN (" . $idlist . ")";
	  	
	  $result = $this->dataDB->query($query);
	  $data = $result->nextAssoc();
	  $result->freeResult();
	  return $data;
	}
      else return array();
    }

  /**
   * \brief Returns Area numbering information 
   *
   * Returns a list of values from the area numbering database :
   * - NbEntries (INT >= 0) : total number of numbering entries
   * - SumValues (INT >= 0) : total sum of numbering entries values
   * - MinTime (DATE) : Min Numbering value timestamp for all the entries in the format YYYYMMDDHHMMSS
   * - MaxTime (DATE) : Max Numbering value timestamp for all the entries in the format YYYYMMDDHHMMSS
   * - SumExpected (INT >= 0) : total number of entries expected by the processing
   * - SumCumul (INT >= 0) : total number of entries processed by the LocalServer
   */
  function getAreaNumberingInfos($params = "")
    {      
      if(!isset($params["timestamp"]))
	{
	  $params["timestamp"] = strftime("%s");
	}     
      if($this->accessDataDB("minute", $params["timestamp"]))    
	{
	  $params["type"] = 2;
	  $zoneprocessing = $this->getZoneNumberingProcessing($params);
	  $idlist = "";
	  foreach($zoneprocessing as $z)
	    {
	      if($idlist != "") $idlist .= ", ";
	      $idlist .= $z["id"];
	    }
	  
	  $query = "select count(*) as NbEntries, sum(value) as SumValues, min(start) as MinTime, "
	    . "max(start) as MaxTime, sum(nbcumul) as SumCumul, sum(nbexpected) as SumExpected from ZoneNumbering_MINUTE "
	    . "WHERE ZoneNumbering_MINUTE.id IN (" . $idlist . ")";
	 
	  $result = $this->dataDB->query($query);
	  $data = $result->nextAssoc();
	  $result->freeResult();	
	  return $data;
	}
      else return array();
    }


  /**
   * \brief Returns Door counting information 
   *
   * Returns a list of values from the door counting database :
   * - NbEntries (INT >= 0) : total number of counters entries
   * - SumValues0 (INT >= 0) : total sum of counters entries values
   * - SumValues1 (INT >= 0) : total sum of counters entries values
   * - MinTime (DATE) : Min Counter value timestamp for all the entries in the format YYYYMMDDHHMMSS
   * - MaxTime (DATE) : Max Counter value timestamp for all the entries in the format YYYYMMDDHHMMSS
   * - SumExpected0 (INT >= 0) : total number of entries expected by the LocalServer
   * - SumExpected1 (INT >= 0) : total number of entries expected by the LocalServer
   * - SumCumul0 (INT >= 0) : total number of entries processed by the LocalServer
   * - SumCumul1 (INT >= 0) : total number of entries processed by the LocalServer
   */
  function getDoorCountingInfos($params = "")
    {
      if(!isset($params["timestamp"]))
	{
	  $params["timestamp"] = strftime("%s");
	}     
     
      if($this->accessDataDB("minute", $params["timestamp"]))      
	{
	  $idlist = "";
	  $params["type"] = 1;
	  foreach ($this->getFlowCountingProcessing($params) as $d)
	    {
	      if($idlist != "") $idlist .= ", ";
	      $idlist .= $d["id"];
	    }

	  $query = "select count(*) as NbEntries, sum(value0) as SumValues0, sum(value1) as SumValues1, min(start) as MinTime, "
	    . "max(start) as MaxTime, sum(nbexpected0) as SumExpected0, sum(nbexpected1) as SumExpected1, "
	    . "sum(nbcumul0) as SumCumul0, sum(nbcumul1) as SumCumul1 from FlowCounting_MINUTE "
	    . "WHERE id IN (" . $idlist . ")";
	  $result = $this->dataDB->query($query);
	  $data = $result->nextAssoc();
	  $result->freeResult();
	  return $data;
	}
      else return array();
    }

  /**
   * \brief Returns Door counting information 
   *
   * Returns a list of values from the door counting database :
   * - NbEntries (INT >= 0) : total number of counters entries
   * - SumValues0 (INT >= 0) : total sum of counters entries values
   * - SumValues1 (INT >= 0) : total sum of counters entries values
   * - MinTime (DATE) : Min Counter value timestamp for all the entries in the format YYYYMMDDHHMMSS
   * - MaxTime (DATE) : Max Counter value timestamp for all the entries in the format YYYYMMDDHHMMSS
   * - SumExpected0 (INT >= 0) : total number of entries expected by the LocalServer
   * - SumExpected1 (INT >= 0) : total number of entries expected by the LocalServer
   * - SumCumul0 (INT >= 0) : total number of entries processed by the LocalServer
   * - SumCumul1 (INT >= 0) : total number of entries processed by the LocalServer
   */
  function getGroupCountingInfos($params = "")
    { 
      if(!isset($params["timestamp"]))
	{
	  $params["timestamp"] = strftime("%s");
	}     
     
      if($this->accessDataDB("minute", $params["timestamp"])) 
	{
	  $params["type"] = 2;
	  $idlist = "";
	  foreach ($this->getFlowCountingProcessing($params) as $d)
	    {
	      if($idlist != "") $idlist .= ", ";
	      $idlist .= $d["id"];
	    }

	  $query = "select count(*) as NbEntries, sum(value0) as SumValues0, sum(value1) as SumValues1, min(start) as MinTime, "
	    . "max(start) as MaxTime, sum(nbexpected0) as SumExpected0, sum(nbexpected1) as SumExpected1, "
	    . "sum(nbcumul0) as SumCumul0, sum(nbcumul1) as SumCumul1 from FlowCounting_MINUTE "
	    . "WHERE id IN (" . $idlist . ")";
	  $result = $this->dataDB->query($query);
	  $data = $result->nextAssoc();
	  $result->freeResult();
	  return $data;
	}
      else return array();
    }

  /**
   * \brief Returns the list of entities with their respective parameters.
   *
   * Returns the list of objects with their parameters contained
   * into the counting processing table.
   * \param $params the array parameters wich contains :
   * - Entity type ('lcation' | 'door' | 'group' | 'area' ) : the type of the wanted Entity
   * - Analysis type (INT >= 0) : the analysis type, only used for the Area entity (will be deprecated).
   */
  function getCountersIds($params) 
    {
      if($params["Entity"] == "location")
	{
	  return $this->getLocationsWithCountingProcessing();
	}
      else if($params["Entity"] == "door")
	{
	  return $this->getDoorsWithCountingProcessing();
	}
      else if($params["Entity"] == "group")
	{
	  return $this->getDoorGroupsWithCountingProcessing();
	}
      else if($params["Entity"] == "area")
	{
	  return $this->getLocationAreasWithCountingProcessing();
	}
      return false;
    }

  function getVisitorsData($params)
    {
      $res = $this->getVisitorsIds($params);
      return $res[$params["id"]];
    }

  function getVisitorsIds($params)
    {
      if($params["Entity"] == "location") $params["type"] = 1;
      else if($params["Entity"] == "area") $params["type"] = 2;
      else return false;
      
      $params["joinedCountingProcessing"] = true;
      return $this->getZones($params);
    }

  function getNumberingData($params)
    {
      $res = $this->getNumberingIds($params);
      return isset($res[$params["id"]]) ? $res[$params["id"]] : null;
    }

  function getCountingData($params)
  {
    $res = $this->getCountersIds($params);
    return $res[$params["id"]];
  }

  function getNumberingIds($params)
    {
      if($params["Entity"] == "location") $params["type"] = 1;
      else if($params["Entity"] == "area") $params["type"] = 2;
      else if($params["Entity"] == "all") $params["type"] = 0;
      else return false;
      
      $params["joinedNumberingProcessing"] = true;
      return $this->getZones($params);
    }

  function getWaitingTimeIds($params)
    {
      if($params["Entity"] == "location") $params["type"] = 1;
      else if($params["Entity"] == "area") $params["type"] = 2;
      else return false;     
      
      $params["joinedWaitingTimeProcessing"] = true;
      return $this->getZones($params);
    }

  /**
   * \brief Insert a new counter value
   * 
   * - clientId (STRING) : the counter client id
   * - channel (INT >= 0) : the channel id
   * - counter (INT >= 0) : the counter id
   * - value (INT >= 0) : the counter value
   * - timeStart (DATE) : the time start in the format YYYYMMDDHHMMSS
   * - timeEnd (DATE) : the time end in the format YYYYMMDDHHMMSS
   */
  function addBTopCounterValue($params = "")
    {
      if(!$params["timeStart"])
	{
	  $params["timeStart"] = $params["timeEnd"];
	}
      $timeStart = strftime("%Y-%m-%d %T", mktimeFromString($params["timeStart"]));
      $ref = $params["clientId"] . "_B-TOP_" . $params["channel"] . "_" . $params["counter"];
      $query = "insert into counting(ref, value, time) values(\"$ref\", " . $params["value"] . ", \"$timeStart\")";
      $result = $this->countDB->query($query);
      $result->freeResult();
      return $params;
    }

  /**
   * \brief Returns the list of the available B-Top sensors
   * 
   * Returns the list of the available B-Top sensors that are in the counter association table that is
   * connected to a defined door. Each element of the list contains : 
   * - id (INT >= 0) : the sensor unique reference 
   * - clientId (STRING) : the unique BlueCount product clientId 
   * - channel (INT >= 0) : the channel id
   * - counter (INT >= 0) : the counter id
   * - host (STRING) : the sensor ip address
   * - descr (STRING) : a description of the sensor
   * - door (STRING) : the name of the door which is connected to this sensor
   * - nameL1 (STRING) : the source location name 
   * _ ref (STRING) : the unique sensor reference
   * - nameL2 (STRING) : the destination location name
   */
  function getBTopSensors($params = "")
    {
      $query = "SELECT C.id AS id, ref, clientId, channel, C.counter AS counter, host, descr, Flows.name AS door, " . 
	"CASE WHEN CountersAssoc.sens=1 THEN L1.name ELSE L2.name END AS nameL1," . 
	"CASE WHEN CountersAssoc.sens=1 THEN L2.name ELSE L1.name END AS nameL2 " .
	"FROM CountersAvailable as C, Locations as L1, Locations as L2, Flows, CountersAssoc " . 
	"WHERE C.id = CountersAssoc.counter AND CountersAssoc.flow = Flows.id AND " .
	"Flows.zone1 = L1.id AND Flows.zone2 = L2.id";
      return $this->get_config_array($query);
    }

  /**
   * \brief Returns the list of the available B-Queue sensors
   *
   * Returns the list of the available B-Qeueu sensors that are in the bqueue Numbering association table
   * that is connected to a defined location. Each element of the list contains : 
   * - id (INT >= 0) : the sensor unique reference
   * - clientId (STRING) : the unique BlueCount product clientId
   * - channel (INT >= 0) : the channel id
   * - region (INT >= 0) : the region id
   * - host (STRING) : the sensor ip address
   * - descr (STRING) : a description of the sensor
   * - ref (STRING) : the complete string reference
   * - location (STRING) : then location name
   */
  function getBQueueSensors($params = "")
    {
      $query = "select N.id as id, ref, clientId, channel, region, host, descr, Locations.name as location from NumberingAvailable as N, Locations, NumberingAssoc where " .
	"N.id = NumberingAssoc.numbering and Locations.id = NumberingAssoc.zone";
      return $this->get_config_array($query);
    }

  /**
   * \brief Returns the list of available zones
   * 
   * \param $params parameter array which contains :
   * - type  0 : all, 1 : simple (location, 2 : complexe (area)
   * - joinedCountingProcessing boolean do or not the jointure to the ZoneCountingProcessing table
   * - joinedNumberingProcessing boolean do or not the jointure to the ZoneNumberingProcessing table
   * Returns the list of the available Zones listed into the location counting
   * processing table. Each element of the list contains :
   * - id (INT >= 0) : the zone unique id 
   * - name (STRING) : the zone name
   * - virtual (0 | 1) : wheter or not the zone is virtual 
   * available if joinedCountingProcessing set to true :
   * - idp (INT >= 0) : the zone counting processing id 
   */
  function getZones($params = "")
    {
      $cond = ""; // all
      $type = isset($params["type"]) ? $params["type"] : 0;
      if($type == 1 || $type == 2)
	{	  
	  $cond = (($type == 1) ? "NOT" : "") . " EXISTS(SELECT * FROM ZoneZoneAssoc AS zza WHERE zza.zone=Zones.id)";
	}

      $compselect = "";
      $compfrom = "";
      $compwhere = "";
      if(isset($params["joinedCountingProcessing"]) and $params["joinedCountingProcessing"])
	{
	  $compselect = "ZoneCountingProcessing.id AS idp, ";
	  $compfrom = "ZoneCountingProcessing, ";
	  $compwhere = "ZoneCountingProcessing.zone = Zones.id";
	}
      else if(isset($params["joinedNumberingProcessing"]) and $params["joinedNumberingProcessing"])
	{
	  $compselect = "ZoneNumberingProcessing.id AS idp, ";
	  $compfrom = "ZoneNumberingProcessing, ";
	  $compwhere = "ZoneNumberingProcessing.zone = Zones.id";
	}
      else if(isset($params["joinedWaitingTimeProcessing"]) and $params["joinedWaitingTimeProcessing"])
	{
	  $compselect = "ZoneWaitingTimeProcessing.id AS idp, Flows.name AS nameFlow, ";
	  $compfrom = "ZoneWaitingTimeProcessing, Flows, ";
	  $compwhere = "ZoneWaitingTimeProcessing.zone = Zones.id AND Flows.id = ZoneWaitingTimeProcessing.flow";
	}

      $query = "SELECT " . $compselect . "Zones.id AS id, Zones.name AS name, Zones.virtual AS virtual "
	. "FROM " . $compfrom . "Zones ";
      if($cond != "" || $compwhere != "")
	{
	  $query .= "WHERE " . $cond;
	  if($cond != "" and $compwhere != "") $query .= " AND ";
	  $query .= $compwhere;
	}

      return $this->get_config_array($query, "id");
    }

  /**
   * \brief Returns the list of available zones counting processing
   *
   * \param params parameters array which contains :
   * - type 0 : all, 1 : simple (location, 2 : complexe (area)
   *
   * Returns the list of available Zone Counting Processing
   * - id (INT >= 0) the processing unique id
   */
  function getZoneCountingProcessing($params = "")
    {
      $cond = ""; //all
      $type = isset($params["type"]) ? $params["type"] : 0;
      if($type == 1 || $type == 2)
	{	  
	  $cond = " WHERE " . (($type == 1) ? "NOT" : "") . " EXISTS(SELECT * FROM ZoneZoneAssoc AS zza WHERE zza.zone=zcp.zone)";
	}

      if($this->configDB->isTableExist("ZoneCountingType") and
	 $this->configDB->isTableExist("FlowCountingType"))
	{
	  $query = "SELECT Zcp.id as id, Zones.id as zoneid, Zones.name as nameZ, " . 
	    "zcp.zc_id as zc_id, ZCT.name as zc_type " .
	    "FROM ZoneCountingProcessing as zcp, Zones, ZoneCountingType as ZCT " . 
	    "where zcp.zone=Zones.id and zcp.zc_id=ZCT.id " . $cond;
	}
      else
	{
	  $query = "SELECT Zcp.id as id, Zones.id as zoneid, Zones.name as nameZ " . 
	    "FROM ZoneCountingProcessing as zcp, Zones " . 
	    "where zcp.zone=Zones.id " . $cond;
	}



      return $this->get_config_array($query, "id");
    }

  /**
   * \brief Returns the list of available zones numbering processing
   *
   * \param params parameters array which contains :
   * - type 0 : all, 1 : simple (location, 2 : complexe (area)
   *
   * Returns the list of available Zone Numbering Processing
   * - id (INT >= 0) the processing unique id
   */
  function getZoneNumberingProcessing($params = "")
    {
      $extraTable = "";
      $cond = ""; //all
      $type = isset($params["type"]) ? $params["type"] : 0;
      if($type == 1 || $type == 2)
	{	  
	  $cond = " AND " . (($type == 1) ? "NOT" : "") . " EXISTS(SELECT * FROM ZoneZoneAssoc AS zza WHERE zza.zone=znp.zone)";
	}
      if(isset($params["withExport"]) and $params["withExport"])
	{
	  $cond .= " and indicator=\"ZoneNumberingProcessing\" and module=\"" . 
	    $params["withExportModule"] . "\" and doExport=1 " .
	    "and Export.id=znp.id";
	  $extraTable = ",Export";
	}
      $query = "SELECT znp.id as id, Zones.id as zoneid, Zones.name as nameZ, numb_id, NumberingType.name as numb_type " . 
	"FROM ZoneNumberingProcessing as znp, Zones, NumberingType" . 
	$extraTable . " WHERE znp.zone=Zones.id and numb_id=NumberingType.id" . $cond;
      if(isset($params["id"])) $query .= " AND znp.id=" . $params["id"];
      return $this->get_config_array($query, "id");
    }

  /**
   * \brief Returns the list of available locations
   *
   * Returns the list of the available Locations listed into the location.
   * Each element of the list contains :
   * - id (INT >= 0) : the location unique id 
   * - name (STRING) : the location name
   * - virtual (0 | 1) : wheter or not the location is virtual 
   */
  function getLocations($params = "")
    {
      return $this->getZones(array("type" => 1));
    }

    /**
     * \brief Returns the list of available locations with counting processing
     *
     * Returns the list of the available Locations listed into the location counting
     * processing table. Each element of the list contains :
     * - id (INT >= 0) : the location unique id 
     * - name (STRING) : the location name
     * - virtual (0 | 1) : wheter or not the location is virtual 
     * - idp : the location processing id
     */
  function getLocationsWithCountingProcessing($params = "")
    {
      $params["type"] = 1;
      $params["joinedCountingProcessing"] = true;
      return $this->getZones($params);
    }

     /**
     * \brief Returns the list of available locations with numbering processing
     *
     * Returns the list of the available Locations listed into the location counting
     * processing table. Each element of the list contains :
     * - id (INT >= 0) : the location unique id 
     * - name (STRING) : the location name
     * - virtual (0 | 1) : wheter or not the location is virtual 
     * - idp : the location numbering processing id
     */
  function getLocationsWithNumberingProcessing($params = "")
    {
      $params["type"] = 1;
      $params["joinedNumberingProcessing"] = true;
      return $this->getZones($params);
    }

  /**
   * \brief Return the list of FlowCountingProcessing
   *
   * \param params parameters array which contains :
   * - type 0 : all, 1 : door, 2 : door group
   *
   * Return the list of the available processing
   * - if (INT >= 0) : the processing id
   */
  function getFlowCountingProcessing($params = "")
    {
      $extraTable = "";
      $cond = "";
      if(isset($params["withExport"]) and $params["withExport"])
	{
	  $cond = " and indicator=\"FlowCountingProcessing\" and module=\"" . 
	    $params["withExportModule"] . "\" and doExport=1 " .
	    "and Export.id=fcp.id";
	  $extraTable = ",Export";
	}
      
      $type = isset($params["type"]) ? $params["type"] : 0;
      if($type == 0) 
	{
	  if($this->configDB->isTableExist("FlowCountingType"))
	    {
	      $pSelect = "SELECT fcp.id as id, Flows.id as flowid, Flows.name as nameF, fcp.fc_id as fc_id, FCT.name as fc_type";
	      $pTable = "FROM FlowCountingProcessing as fcp, FlowCountingType as FCT, Flows"; 
	      $part2 = " where fcp.flow=Flows.id and fcp.fc_id=FCT.id";
	    }
	  else
	    {
	      $pSelect = "SELECT fcp.id as id, Flows.id as flowid, Flows.name as nameF";
	      $pTable = "FROM FlowCountingProcessing as fcp, Flows"; 
	      $part2 = " where fcp.flow=Flows.id";
	    }
	  $part3 = "EXISTS (SELECT * from FlowFlowAssoc where " . 
	    "FlowFlowAssoc.flow=fcp.flow)";
	  $query = $pSelect . ",'door' as entity " . $pTable . $extraTable . $part2 . 
	    $cond . " AND NOT " . $part3 . " UNION " .
	    $pSelect . ",'group' as entity " . $pTable . $part2 . " AND " . $part3;
	}
      else
	{
	  $cond .= " AND " . (($type == 1) ? "NOT" : "") . 
	    " EXISTS (SELECT * FROM FlowFlowAssoc " . 
	    "WHERE FlowFlowAssoc.flow = fcp.flow)";
	  if($this->configDB->isTableExist("FlowCountingType"))
	    {	      
	      $query = "SELECT fcp.id as id , Flows.id as flowid, Flows.name as nameF, fcp.fc_id as fc_id, FCT.name as fc_type " . 
		"FROM FlowCountingProcessing as fcp, FlowCountingType as FCT, Flows" . 
		$extraTable . " where fcp.flow=Flows.id and fcp.fc_id=FCT.id" . $cond;
	    }
	  else
	    {
	      $query = "SELECT fcp.id as id , Flows.id as flowid, Flows.name as nameF " . 
		"FROM FlowCountingProcessing as fcp, Flows" . 
		$extraTable . " where fcp.flow=Flows.id" . $cond;
	    }
	}
	  
      if(isset($params["id"])) $query .= " AND Flows.id = " . $params["id"];
      
      return $this->get_config_array($query, "id");
    }

  /**
   * \return the list of available flows with possibility of filters
   *
   * \params $params parameters array which can contains
   * - type 0 : all, 1, simple flows (doors), 2 : complexe flows (door group)
   * - joinedCountingProcessing if true do the jointure to the FlowCountingProcessing
   * - id  : to get a specific flow
   *
   * Returns the list of available flows. Each element of the list contains : 
   * - id (INT >= 0) : the flow unique id
   * - name (STRING) : the flow name
   * - idZ1 : the first zone id
   * - idZ2 : the second zone id
   * - nameZ1 : the first zone name
   * - nameZ2 : the second zone name
   * available if joinedCountingProcessing set to true
   * - idp : the flow counting processing id  
   */
  function getFlows($params = "")
    {
      $typecond = "";
      if(isset($params["type"]) and ($params["type"] == 1 || $params["type"] == 2))
	{
	  $typecond=(($params["type"] == 1) ? "NOT" : "") . " EXISTS(SELECT * FROM FlowFlowAssoc WHERE flow=Flows.id)";
	}
      
      $processingSelect = "";
      $processingFrom = "";
      $processingWhere = "";
      if(isset($params["joinedCountingProcessing"]) and $params["joinedCountingProcessing"])
	{
	  $processingSelect = "FlowCountingProcessing.id as idp, ";
	  $processingFrom = "FlowCountingProcessing, ";
	  $processingWhere = "FlowCountingProcessing.flow = Flows.id";
	}
      
      $query = "SELECT " . $processingSelect . "Flows.id as id," .
	" Flows.name as name, Flows.zone1 as idZ1, Flows.zone2 as idZ2, Z1.name as nameZ1, Z2.name as nameZ2" .
	" FROM " . $processingFrom . " Flows, Zones AS Z1, Zones AS Z2 " .
	" WHERE Flows.zone1 = Z1.id and Flows.zone2 = Z2.id ";
      
      if($typecond != "") $query .= " AND " . $typecond;
      if($processingWhere != "") $query .= " AND " . $processingWhere;

      if(isset($params["id"])) $query .= " AND Flows.id = " . $params["id"];

      //var_dump($query);

      return $this->get_config_array($query, "id");
    }

  /**
   * \brief Returns the list of available doors
   *
   * Returns the list of available doors. Each element of the list contains : 
   * - id (INT >= 0) : the door unique id
   * - name (STRING) : the door name
   * - nameZ1 : the first location name
   * - nameZ2 : the second location name
   */ 
  function getDoors($params = "")
    {
      return $this->getFlows(array("type" => 1, "joinedCountingProcessing" => false));				   
    }

  function getDoorsWithCountingProcessing($params = "")
    {
      $params["type"] = 1;
      $params["joinedCountingProcessing"] = true;
      return $this->getFlows($params);
    }  

  function getDoorGroupsWithCountingProcessing($params = "")
    {
      $params["type"] = 2;
      $params["joinedCountingProcessing"] = true;
      return $this->getFlows($params);
    }

  function getFlowsWithCountingProcessing($params = "")
    {
      $params["type"] = 0;
      $params["joinedCountingProcessing"] = true;
      return $this->getFlows($params);
    }

  /**
   * \brief Returns the list of available oriented doors
   *
   * Returns the list of available oriented doors. Each element of the list contains :
   * - id (INT >= 0) : the unique oriented door id
   * - door (STRING) : the door name which is connected to this oriented door
   * - l1 (STRING) : the source location name
   * - l2 (STRING) : the destination location name
   */
  function getOrientedDoors($params = "")
    {
      $query = "SELECT Doors.id AS id, 1 AS sens, Doors.name as door, L1.name AS nameL1, L2.name AS nameL2 ".
	"FROM Doors, Zones AS L1, Zones AS L2 " .
	"WHERE Doors.zone1 = L1.id and Doors.zone2 = L2.id union " .
	"SELECT Doors.id AS id, 2 AS sens, Doors.name as door, L2.name AS nameL1, L1.name AS nameL2 ".
	"FROM Doors, Zones AS L1, Zones AS L2 " .
	"WHERE Doors.zone1 = L1.id and Doors.zone2 = L2.id";


      return $this->get_config_array($query);
    }

  /**
   * \brief Returns the list of all possible location pairs
   *
   * Returns the list of all possible location pairs. Each element of the list contains :
   * - idL1 (INT >= 0) : the first location id
   * - idL2 (INT >= 0) : the second location id
   * - nameL1 (STRING) : the first location name
   * - nameL2 (STRING) : the second location name
   * For example, given the following locations :
   * A, B, C, D
   * this returns the following list of pairs :
   * ( A, B ), (A, C), (A, D),
   * ( B, A ), (B, C), (B, D)
   * ( C, A ), (C, B), (C, D)
   */
  function getLocationPairs($params = "")
    {
      $query = "SELECT L1.id AS idL1, L2.id AS idL2, L1.name AS nameL1, L2.name AS nameL2 "
	. "FROM Locations AS L1, Locations AS L2 WHERE L2.id != L1.id ORDER BY L1.id ASC";
      return $this->get_config_array($query);
    }

  /**
   * \brief Return list of all possible zone pairs linked per door
   *
   * Return the list of all possible pairs of Zone : mis of area and location
   * Each element of the list contains :
   * - idZ1 (INT >= 0) : the first zone id
   * - idZ2 (INT >= 0) : the second zone id
   * - nameZ1 (STRING) : the name of the first zone
   * - nameZ2 (STRING) : the name of the second zone
   */
  function getZonePairsWhereDoor($params = "")
    {
      $query = "SELECT Z1.id AS idZ1, Z2.id AS idZ2, Z1.name AS nameZ1, Z2.name AS nameZ2 "
	. "FROM Zones AS Z1, Zones AS Z2 WHERE Z2.id != Z1.id AND EXISTS ( " 
	. "SELECT * FROM Flows WHERE NOT EXISTS(SELECT * FROM FlowFlowAssoc WHERE flow = Flows.id ) AND " 
	. "(zone1 IN (SELECT contained FROM ZoneZoneResolvedAssoc WHERE zone=Z1.id) AND " 
	. "zone2 IN (SELECT contained FROM ZoneZoneResolvedAssoc WHERE zone=Z2.id) AND "
	. "zone2 NOT IN (SELECT contained FROM ZoneZoneResolvedAssoc WHERE zone=Z1.id) AND "
	. "zone1 NOT IN (SELECT contained FROM ZoneZoneResolvedAssoc WHERE zone=Z2.id)) OR "
	. "(zone2 IN (SELECT contained FROM ZoneZoneResolvedAssoc WHERE zone=Z1.id) AND " 
	. "zone1 IN (SELECT contained FROM ZoneZoneResolvedAssoc WHERE zone=Z2.id) AND "
	. "zone1 NOT IN (SELECT contained FROM ZoneZoneResolvedAssoc WHERE zone=Z1.id) AND "
	. "zone2 NOT IN (SELECT contained FROM ZoneZoneResolvedAssoc WHERE zone=Z2.id))"
	. ") "
	. "ORDER BY Z1.id ASC";
      return $this->get_config_array($query);
    }

  /**
   * \brief Returns the list of all possible location pairs from the doors list
   *
   * Returns the list of all possible location pairs contained into the doors list that is
   * every location pairs that match at least one door.
   * Each element of the list contains :
   * - idL1 (INT >= 0) : the first location id
   * - idL2 (INT >= 0) : the second location id
   * - l1 (STRING) : the first location name
   * - l2 (STRING) : the second location name
   */
  function getLocationPairsFromDoors($params = "")
    {
      $query = "select L1.name as nameL1, L2.name as nameL2, Doors.idL1 as idL1, Doors.idL2 as idL2 from Doors, " .
	"Locations as L1, Locations as L2 where Doors.idL1 = L1.id and Doors.idL2 = L2.id group by idL1, idL2";
      return $this->get_config_array($query);
    }

  /**
   * \brief Returns the list of available door groups
   *
   * Return the list of available door groups
   *
   * Each element of the list contains :
   * - id (INT >= 0) : the group unique id
   * - name (STRING) : the group name
   * - nameZ1 : the first zone name
   * - nameZ2 : the second zone name
   */
  function getDoorGroups($params = "")
    {
      return $this->getFlows(array("type" => 2, "joinedCountingProcessing" => false));
    }

  /**
   * \brief Returns the list of available areas
   *
   * Return the list of available areas that are in the
   * area location association.
   * Each element of the list contains :
   * - id (INT >= 0) : the area unique id
   * - name (STRING) : the area name
   */
  function getLocationAreas($params = "")
    {
      return $this->getZones(array("type" => 2));
    }

  /**
   * \brief Returns the list of available areas
   *
   * Return the list of available areas that are in the
   * area location association.
   * Each element of the list contains :
   * - id (INT >= 0) : the area unique id
   * - name (STRING) : the area name
   * - idp (INT >= 0) : the area counting processing
   */
  function getLocationAreasWithCountingProcessing($params = "")
    {
      $params["type"] = 2;
      $params["joinedCountingProcessing"] = true;
      return $this->getZones($params);
    }

  /**
   * \brief Returns the list of available zone (location / area) with numbering processing
   *
   * Return the list of available location or areas that are in the numbering processing
   * Each element of the list contains :
   * - id (INT >= 0) : the zone unique id
   * - name (STRING) : the zone name
   * - idp (INT >= 0) : the area numbering processing
   */
  function getZonesWithNumberingProcessing($params = "")
    {
      $params["type"] = 0;
      $params["joinedNumberingProcessing"] = true;
      return $this->getZones($params);
    }

  function getZonesWithCountingProcessing($params = "")
  {
    $params["type"] = 0;
    $params["joinedNumberingProcessing"] = true;
    return $this->getZones($params);
  }

  /**
   * \brief Returns the indicators properties for the zone waiting time processing
   * \param $params parameter array
   * - id optionnal : areawaitingtime id
   *
   * Each element of the list contains :
   * - idp (INT >= 0) : the unique processing id
   * - zoneid (INT >= 0) : the unique zone id
   * - flowid (INT >= 0) : the unique flow id
   * - sens (INT = 1 | 2) : the group sens
   * - nameZ (STRING) : the zone name (area/location name)
   * - nameF (STRING) : the flow name (door/ door group)
   * - nameZ1 (STRING) : the flow start location name
   * - nameZ2 (STRING) : the flow end location name
   */
  function getZoneWaitingTimeProcessing($params = "")
    {      
      $extraTable = "";
      $cond = "";
      if(isset($params["withExport"]) and $params["withExport"])
	{
	  $cond = " and indicator=\"ZoneWaitingTimeProcessing\" and module=\"" . 
	    $params["withExportModule"] . "\" and doExport=1 " .
	    "and Export.id=zwtp.id";
	  $extraTable = ",Export";
	}
      $query = "SELECT zwtp.id as id, zwtp.zone as zoneid, zwtp.flow as flowid, Zones.name as nameZ, Flows.name as nameF, zwtp.sens as sens," .
	" CASE WHEN zwtp.sens = 1 THEN Z1.name ELSE Z2.name END AS nameZ1, " .
	" CASE WHEN zwtp.sens = 1 THEN Z2.name ELSE Z1.name END AS nameZ2, zwtp.wt_id AS wt_id, WTT.name as wt_type, WTT.name as wt_name" . 
	" FROM ZoneWaitingTimeProcessing as zwtp, Zones, Zones AS Z1, Zones AS Z2, Flows, WaitingTimeType as WTT" . 
	$extraTable . " WHERE " . 
	" Zones.id = zwtp.zone AND zwtp.flow = Flows.id AND Z1.id = Flows.zone1 AND Z2.id = Flows.zone2 AND zwtp.wt_id=WTT.id" . $cond;
      if(isset($params["id"])) $query .= " AND zwtp.id = " . $params["id"];

      return $this->get_config_array($query);
    }

  function getZoneProcessTimeProcessing($params = "")
  {
    if($this->configDB->isTableExist("ProcessTimeType"))
      {
	$query = "SELECT zptp.id as id, zptp.zone as zoneid, zptp.flow as flowid, Zones.name as nameZ, Flows.name as nameF, zptp.sens as sens," .
	  " CASE WHEN zptp.sens = 1 THEN Z1.name ELSE Z2.name END AS nameZ1, " .
	  " CASE WHEN zptp.sens = 1 THEN Z2.name ELSE Z1.name END AS nameZ2, zptp.pt_id AS pt_id, PTT.name as pt_name" . 
	  " FROM ZoneProcessTimeProcessing as zptp, Zones, Zones AS Z1, Zones AS Z2, Flows, ProcessTimeType as PTT WHERE " . 
	  " Zones.id = zptp.zone AND zptp.flow = Flows.id AND Z1.id = Flows.zone1 AND Z2.id = Flows.zone2 AND zptp.pt_id=PTT.id";
	if(isset($params["id"])) $query .= " AND zptp.id = " . $params["id"];
	return $this->get_config_array($query);
      }
    else
      {
	return array();
      }
  }

  /**
   * \brief Returns the parameters of a specified B-Top sensor
   *
   * Returns the parameters of a specified B-Top sensor which is in the counter
   * association list. 
   * \param $params the parameters array which contains : 
   * - id (INT >= 0) : the unique B-Top sensor id
   * Then the function returns an array that contains : 
   * - id (INT >= 0) : the unique B-Top sensor id
   * - clientId (STRING) : the B-Top sensor client id
   * - channel (INT >= 0) : the channel id
   * - channel (INT >= 0) : the counter id
   * - host (STRING) : the B-Top sensor ip address
   * - descr (STRING) : the sensor description
   * - door (INT >= 0) : the door unique id which is connected 
   * - sens (INT 1 / 2)à : the sens for the door association
   */
  function getBTopSensorData($params)
    {
      $query = "select id, clientId, channel, CountersAvailable.counter, host, descr, CountersAssoc.flow as door, CountersAssoc.sens as sens " .
	"from CountersAvailable, CountersAssoc where id=" . $params["id"] .
	" and CountersAssoc.counter=" . $params["id"];
      return $this->get_config_array($query);
    }

  /**
   * \brief Returns the parameters of a specified B-Queue sensor
   *
   * Returns the parameters of a specified B-Queue sensor which is in the numbering
   * association list. 
   * \param $params the parameters array which contains : 
   * - id (INT >= 0) : the unique sensor id
   * - clientId (STRING) : the B-Queue sensor client id
   * - channel (INT >= 0) : the channel id
   * - region (INT >= 0) : the region id
   * - host (STRING) : the B-Queue sensor ip address
   * - descr (STRING) : the sensor description
   * - location (INT >= 0) : the location unique id which is connected 
   */
  function getBQueueSensorData($params)
    {
      $query = "select id, clientId, channel, region, host, descr, NumberingAssoc.zone as location from NumberingAvailable, NumberingAssoc where id=" . $params["id"] .
	" and NumberingAssoc.numbering=" . $params["id"];
      return $this->get_config_array($query);
    }


  /**
   * \brief Returns the number of BTopSensors available
   */
  function getNumberOfBTopSensors($params = "")
    {
      return $this->get_number_of_entries("CountersAvailable");
    }

  /**
   * \brief Returns the number of Locations
   */
  function getNumberOfLocations($params = "")
    {
      return $this->get_number_of_entries("Zones", "WHERE NOT EXISTS(SELECT * FROM ZoneZoneResolvedAssoc AS zzra WHERE zzra.contained != zzra.zone AND zzra.zone=Zones.id)");
    }

  /**
   * \brief Returns the number of Doors
   */
  function getNumberOfDoors($params = "")
    {      
      return $this->get_number_of_entries("Flows", "WHERE NOT EXISTS(SELECT * FROM FlowFlowResolvedAssoc AS ffra WHERE ffra.contained != ffra.flow AND ffra.flow=Flows.id)");
    }

  /**
   * \brief Returns the numner of Areas
   */
  function getNumberOfAreas($params = "")
    {
      return $this->get_number_of_entries("Zones", "WHERE EXISTS(SELECT * FROM ZoneZoneResolvedAssoc AS zzra WHERE zzra.contained != zzra.zone AND zzra.zone=Zones.id)");
    }

  /**
   * \brief Returns the number of Areas for the numbering processing
   */
  function getNumberOfNumberingAreas($params = "")
    {
      return $this->get_number_of_entries("ZoneNumberingProcessing");
    }

  /**
   * \brief Returns the numner of Groups
   */
  function getNumberOfGroups($params = "")
    {
      return $this->get_number_of_entries("Flows", "WHERE EXISTS(SELECT * FROM FlowFlowResolvedAssoc AS ffra WHERE ffra.contained != ffra.flow AND ffra.flow=Flows.id)");
    }

  /**
   * \brief Changes the parameters values of the specified B-Top sensor.
   * 
   * Changes the parameters values of the specified B-Top sensor which is in the counter
   * association list.
   * \param $params the parameters array which contains :
   * - id (STRING) : the unique sensor id
   * - clientId (STRING) : the new client id of the sensor
   * - channel (INT >= 0) : then new channel id
   * - counter (INT >= 0) : the new counter id
   * - host (STRING) : the new sensor ip address
   * - descr (STRING) : the new sensor description
   * - door (INT >= 0) : the new oriented door unique id which is connected
   * \returns the new sensor parameters data in an array list @see getBTopSensorData
   */
  function processBTopSensorUpdate($params)
    {
      // will be deprecated
      $ref = $params["clientId"] . "_B-TOP_" . $params["channel"] . "_" . $params["counter"];

      $query = "update CountersAvailable set host=\"" . $params["host"] . "\"" .
	",clientId=\"" . $params["clientId"] . "\"" .
	",channel=" . $params["channel"] .
	",counter=" . $params["counter"] .
	",descr=\"" . $params["descr"] . "\"" .
	",type=\"B-TOP\"" . 
	",ref=\"" . $ref . "\"" .
	" where id=" . $params["id"];
      $result = $this->configDB->query($query);
      $result->freeResult();
 
      $query = "update CountersAssoc set flow=" . $params["door"] . ", sens=" . $params["sens"] ." where counter=" . $params["id"];
      $result = $this->configDB->query($query);
      $result->freeResult();

      return array( array( $params["id"], $params["clientId"], $params["channel"], $params["counter"], $params["host"], $params["descr"], "type" => "B-TOP", $params["door"] ) );
    }

  /**
   * \brief Inserts a new B-Top sensor.
   * 
   * Inserts a new B-Top sensor with its connected oriented door.
   * \param $params the parameters array which contains : 
   * - clientId (STRING) : the B-Top sensor client id
   * - channel (INT >= 0) : the channel id
   * - counter (INT >= 0) : the counter id
   * - host (STRING) : an ip address
   * - descr (STRING) : a description
   * - door (INT >= 0) : the oriented door existing id which is connected
   * \returns the new sensor parameters data in an array list @see getBTopSensorData
   */
  function processBTopSensorInsert($params)
    {
      // get a new counter id
      $query = "select ifnull(max(id) + 1, 0) as id from CountersAvailable";
      $result = $this->configDB->query($query);
      $obj = $result->nextAssoc();
      $result->freeResult();  
      $params["id"] = $obj["id"];

      $ref = $params["clientId"] . "_B-TOP_" . $params["channel"] . "_" . $params["counter"];

      $query = "insert into CountersAvailable values(" . $params["id"] . 
	",\"" . $params["clientId"] . "\"," . $params["channel"] . "," . $params["counter"] . ",\"" . $params["host"] . "\",\"" . $params["descr"] . "\",\"B-TOP\",\"" . $ref . "\")";
      $result = $this->configDB->query($query);
      $result->freeResult();
      
      $query = "insert into CountersAssoc(counter, flow, sens) values(" . $params["id"] . "," . $params["door"] . "," . $params["sens"] . ")";
      $result = $this->configDB->query($query);
      $result->freeResult();

      return array( "id" => $params["id"], 
		    "clientId" => $params["clientId"], 
		    "channel" => $params["channel"], 
		    "counter" => $params["counter"], 
		    "host" => $params["host"], 
		    "type" => "B-TOP", 
		    "descr" => $params["descr"], 
		    "door" => $params["door"] );
    }

  /**
   * \brief Delete an existing B-Top sensor.
   * 
   * Delete the existing B-Top sensor with its connection to the oriented door.
   * \param $params the parameters array which contains : 
   * - id (INT >= 0) : an unique sensor id
   * \returns the reference of the deleted sensor in an array list @see getBTopSensorData
   */
  function processBTopSensorDelete($params)
    {
      $query = "delete from CountersAvailable where id=" . $params["id"];
      $result = $this->configDB->query($query);
      $result->freeResult();
      
      return array( array( $params["id"] ) );
    }

  /**
   * \brief Changes the parameters values of the specified B-Queue sensor.
   * 
   * Changes the parameters values of the specified B-Queue sensor which is in the numbering
   * association list.
   * \param $params the parameters array which contains :
   * - id (STRING) : the unique sensor id
   * - clientId (STRING) : the new client id of the sensor
   * - channel (INT >= 0) : then new channel id
   * - region (INT >= 0) : the new region id (optional, default : 0)
   * - host (STRING) : the new sensor ip address
   * - descr (STRING) : the new sensor description
   * - location (INT >= 0) : the new location unique id which is connected
   * \returns the new sensor parameters data in an array list @see getBQueueSensorData
   */
  function processBQueueSensorUpdate($params)
    { 
      if(!isset($params["region"])) $params["region"] = 0;
      
      // will be deprecated
      $ref = $params["clientId"] . "_B-QUEUE_" . $params["channel"] . "_" . $params["region"];   

      $query = "update NumberingAvailable set host=\"" . $params["host"] . "\"" .
	",clientId=\"" . $params["clientId"] . "\"" .
	",channel=" . $params["channel"] .
	",region=" . $params["region"] .
	",descr=\"" . $params["descr"] . "\"" .
	",type=\"B-QUEUE\"" . 
	",ref=\"" . $ref . "\"" .
	" where id=" . $params["id"];
      $result = $this->configDB->query($query);
      $result->freeResult();
      
      $query = "update NumberingAssoc set zone=" . $params["location"] . " where numbering=" . $params["id"];
      $result = $this->configDB->query($query);
      $result->freeResult();

      return array( array( $params["id"], $params["clientId"], $params["channel"], $params["region"], $params["host"], $params["descr"], "type" => "B-QUEUE", $params["location"] ) );
    }

  /**
   * \brief Inserts a new B-Queue sensor.
   * 
   * Inserts a new B-Queue sensor with its connected location.
   * \param $params the parameters array which contains : 
   * - clientId (STRING) : the B-Top sensor client id
   * - channel (INT >= 0) : the channel id
   * - region (INT >= 0) : the region id (optional, default 0)
   * - host (STRING) : an ip address
   * - descr (STRING) : a description
   * - location (INT >= 0) : the location door existing id which is connected
   * \returns the new sensor parameters data in an array list @see getBQueueSensorData
   */
  function processBQueueSensorInsert($params)
    {
      // get a new counter id
      $query = "select ifnull(max(id) + 1, 0) as id from NumberingAvailable";
      $result = $this->configDB->query($query);
      $obj = $result->nextAssoc();
      $result->freeResult();  
      $params["id"] = $obj["id"];

      if(!isset($params["region"])) $params["region"] = 0;

      $ref = $params["clientId"] . "_B-QUEUE_" . $params["channel"] . "_" . $params["region"];

      $query = "insert into NumberingAvailable values(" . $params["id"] . 
	",\"" . $params["clientId"] . "\"," . $params["channel"] . "," . $params["region"] . 
	",\"" . $params["host"] . "\",\"" . $params["descr"] . "\",\"B-QUEUE\",\"" . $ref . "\")";
      $result = $this->configDB->query($query);
      $result->freeResult();
      
      $query = "insert into NumberingAssoc(numbering, zone) values(" . $params["id"] . "," . $params["location"] . ")";
      $result = $this->configDB->query($query);
      $result->freeResult();

      return array( array( $params["id"], $params["clientId"], $params["channel"], $params["region"], $params["host"], "type" => "B-QUEUE", $params["descr"], $params["location"] ) );
    }

  /**
   * \brief Delete an existing B-Queue sensor.
   * 
   * Delete the existing B-Queue sensor with its connection to the location.
   * \param $params the parameters array which contains : 
   * - id (INT >= 0) : an unique sensor id
   * \returns the reference of the deleted sensor in an array list @see getBQueueSensorData
   */
  function processBQueueSensorDelete($params)
    {
      $query = "delete from NumberingAvailable where id=" . $params["id"];
      $result = $this->configDB->query($query);
      $result->freeResult();

      return array( array( $params["id"] ) );
    }
  
  /**
   * \brief Returns the parameters of a specified Zone
   *
   * Returns the parameters of a specified Zone
   * \param $params the parameters array which contains : 
   * - id (INT >= 0) : the unique zone id
   * \returns an array that contains : 
   * - id (INT >= 0) : the unique zone id
   * - name (STRING) : the zone name
   * - virtual ('true' | 'false') : if the zone is virtual
   */
  function getZoneData($params)
    {
      $query = "SELECT id, name, virtual FROM Zones where id=" . $params["id"];
      return $this->get_config_array($query);
    }

  /**
   * \brief Changes the parameters values of the specified Zone
   * 
   * Changes the parameters values of the specified Zone
   * \param $params the parameters array which contains : 
   * - id (INT >= 0) : the unique zone id
   * - name (STRING) : the new zone name
   * - virtual (1 : 'true' | 0 : 'false') : the new virtual value
   * \returns the new zone parameters data in an array list @see getZoneData
   */
  function processZoneUpdate($params)
    {
      $query = "update Zones set name=\"" . $params["name"] . "\"" .
	(isset($params["virtual"]) ? ",virtual=" .  $params["virtual"] : "") . 
	" where id=" . $params["id"];
      $result = $this->configDB->query($query);
      $result->freeResult();
      return array( array( $params["id"], $params["name"], $params["virtual"] ) );
    }

  /**
   * \brief Inserts a new Zone.
   * 
   * Inserts a new Zone and automatically add it to the zone
   * counting processing table. Creates an unique id automatically.
   * \param $params the parameters array which contains : 
   * - name (STRING) : the new location name
   * - virtual (1 : 'true'  | 0 : 'false') : the new virtual value
   * \returns the new zone parameters data in an array list @see getZoneData
   */
  function processZoneInsert($params)
    {
      // get a new location id
      $query = "SELECT IFNULL(MAX(id) + 1, 0) AS id FROM Zones";
      $result = $this->configDB->query($query);
      $obj = $result->nextAssoc();
      $result->freeResult();  
      $params["id"] = $obj["id"];

      // creates the new location
      $query = "INSERT INTO Zones(id, name, virtual) VALUES(" . $params["id"] .  ",\"" . $params["name"] . "\", " . (isset($params["virtual"]) ? $params["virtual"] : "0") . ")";

      $result = $this->configDB->query($query);
      $result->freeResult();  

      $this->processVisitorsInsert($params);
      $this->processNumberingInsert($params);

      return array( "id" => $params["id"], "name" => $params["name"], "virtual" => $params["virtual"] );
    }

  /**
   * \brief Create a visitor indicator for the specified zone
   *
   * \param $params the parameters array which contains :
   * - id (INT) : the unique zone id
   * \returns the zone reference
   */
  function processVisitorsInsert($params)
    {
      $query = "INSERT INTO ZoneCountingProcessing(calendar, zone) SELECT Calendars.id, " . $params["id"] . " FROM Calendars LIMIT 1;";
      $result = $this->configDB->query($query);
      $result->freeResult();        
      return array( $params );
    }

  /**
   * \brief Delete a visitor indicator for the specified zone
   * 
   * \param $params the parameters array which contains :
   * - id (INT) : the unique zone id
   * \returns the zone reference
   */
  function processVisitorsDelete($params)
    {
      $query = "DELETE FROM ZoneCountingProcessing WHERE zone=" . $params["id"];
      $result = $this->configDB->query($query);
      $result->freeResult();  
      return array( $params );
    }

  /**
   * \brief Create a numbering indicator for the specified zone
   *
   * \param $params the parameters array which contains :
   * - id (INT) : the unique zone id
   * \returns the zone reference
   */
  function processNumberingInsert($params)
    {
      $query = "INSERT INTO ZoneNumberingProcessing(calendar, zone) SELECT Calendars.id, " . $params["id"] . " FROM Calendars LIMIT 1;";
      $result = $this->configDB->query($query);
      $result->freeResult();        
      return array( $params );
    }

  /**
   * \brief Delete a numbering indicator for the specified zone
   * 
   * \param $params the parameters array which contains :
   * - id (INT) : the unique zone id
   * \returns the zone reference
   */
  function processNumberingDelete($params)
    {
      $query = "DELETE FROM ZoneNumberingProcessing WHERE zone=" . $params["id"];
      $result = $this->configDB->query($query);
      $result->freeResult();  
      return array( $params );
    }

 /**
   * \brief Delete an existing Zone.
   * 
   * Delete the existing Zone.
   * \param $params the parameters array which contains : 
   * - id (INT >= 0) : an unique zone id
   * \returns the reference of the deleted zone in an array list @see getLocationData
   */
  function processZoneDelete($params)
    {
      $query = "DELETE FROM Zones WHERE id=" . $params["id"];
      $result = $this->configDB->query($query);
      $result->freeResult();  
      return array( array( $params["id"] ) );
    }


  /**
   * \brief Returns the parameters of a specified Location
   *
   * Returns the parameters of a specified Location
   * \param $params the parameters array which contains : 
   * - id (INT >= 0) : the unique location id
   * \returns an array that contains : 
   * - id (INT >= 0) : the unique location id
   * - name (STRING) : the location name
   * - virtual ('true' | 'false') : if the location is virtual
   */
  function getLocationData($params)
    {
      return $this->getZoneData($params);
    }

  /**
   * \brief Changes the parameters values of the specified Location
   * 
   * Changes the parameters values of the specified Location
   * \param $params the parameters array which contains : 
   * - id (INT >= 0) : the unique location id
   * - name (STRING) : the new location name
   * - virtual (1 : 'true' | 0 : 'false') : the new virtual value
   * \returns the new location parameters data in an array list @see getLocationData
   */
  function processLocationUpdate($params)
    {
      return $this->processZoneUpdate($params);
    }

  /**
   * \brief Inserts a new Location.
   * 
   * Inserts a new Location and automatically add it to the location
   * counting processing table. Creates an unique id automatically.
   * \param $params the parameters array which contains : 
   * - name (STRING) : the new location name
   * - haveSnapshot ('true' | 'false') : if this location have a snapshot ?
   * - posMap (STRING) : position on map
   * - virtual ('true' | 'false') : the new virtual value
   * - area ('true' | 'false') : ???
   * \returns the new location parameters data in an array list @see getLocationData
   */
  function processLocationInsert($params)
    {
      return $this->processZoneInsert($params);
    }

  /**
   * \brief Delete an existing Location.
   * 
   * Delete the existing Location.
   * \param $params the parameters array which contains : 
   * - id (INT >= 0) : an unique location id
   * \returns the reference of the deleted location in an array list @see getLocationData
   */
  function processLocationDelete($params)
    {
      return $this->processZoneDelete($params);
    }

  /**
   * \brief Returns the parameters of a specified door
   *
   * Returns the parameters of a specified Door
   * \param $params the parameters array which contains : 
   * - id (INT >= 0) : the unique door id
   * \returns an array that contains : 
   * - id (INT >= 0) : the unique door id
   * - name (STRING) : the door name
   * - nameZ1 (STRING) : the first location name
   * - nameZ2 (STRING) : the second location name
   * - idZ1 (INT >= 0) : the first unique location id
   * - idZ2 (INT >= 0) : the second unique location id
   */
  function getDoorData($params)
    {
      $res = $this->getFlows($params);     
      return $res[$params["id"]];
   }

  /**
   * \brief Changed the parameters values of the specified flow
   *
   * \param $params the parameters array which contains : 
   * - id (INT >= 0) : the unique door id
   * - name (STRING) : the new door name
   * - idZ1 (INT >= 0) : the first location id
   * - idZ2 (INT >= 0) : the second location id 
   *
   * \returns the new door parameters data in an array list that contains :
   * - id (INT >= 0) : the unique door id
   * - name (STRING) : the new name
   * - idZ1 (INT >= 0) : the new first location id
   * - idZ2 (INT >= 0) : the new second location id
   */
  function processFlowUpdate($params)
    {
      $query = "UPDATE Flows SET name=\"" . $params["name"] . "\"" .
	", zone1=" . $params["idZ1"] . ", zone2=" . $params["idZ2"] .
	" WHERE id=" . $params["id"];
      $result = $this->configDB->query($query);
      $result->freeResult();
      return array( array( $params["id"], $params["name"], $params["idZ1"], $params["idZ2"] ) );
    }

  /**
   * \brief Inserts a new flow.
   * 
   * Inserts a new Flow and automatically add it to the flow
   * counting processing table. Creates an unique id automatically.  
   * \param $params the parameters array which contains : 
   * - name (STRING) : the new flow name
   * - idZ1 (INT >= 0) the "from" location
   * - idZ2 (INT >= 0) the "to" location
   * \returns the new door parameters data in an array list @see processDoorUpdate
   */
  function processFlowInsert($params)
    {
      // get a new door id
      $query = "SELECT IFNULL(MAX(id) + 1, 0) AS id FROM Flows";
      $result = $this->configDB->query($query);
      $obj = $result->nextAssoc();
      $result->freeResult();  
      $params["id"] = $obj["id"];

      // creates the new door
      $query = "INSERT INTO Flows(id, name, zone1, zone2) VALUES(" . $params["id"] . ", \"" . $params["name"] . "\"" .
	", " . $params["idZ1"] . ", " . $params["idZ2"] . " );";
      $result = $this->configDB->query($query);
      $result->freeResult();

      // insert automatically in counting processing
      $query = "INSERT INTO FlowCountingProcessing(flow, calendar) SELECT " . $params["id"] . ", Calendars.id FROM Calendars LIMIT 1;";
      $result = $this->configDB->query($query);
      $result->freeResult();  

      return array( "id" => $params["id"], "name" => $params["name"], "idZ1" => $params["idZ1"], "idZ2" => $params["idZ2"] );
    }

  /**
   * \brief Delete an existing Flow.
   * 
   * Delete the existing Flow.
   * \param $params the parameters array which contains : 
   * - id (INT >= 0) : an unique door id
   * \returns the reference of the deleted flow in an array list 
   */
  function processFlowDelete($params)
    {
      $query = "DELETE FROM Flows WHERE id=" . $params["id"];
      $result = $this->configDB->query($query);
      $result->freeResult();  
      return array( array( $params["id"] ) );
    }

  /**
   * \brief Changes the parameters values of the specified door
   * 
   * Changes the parameters values of the specified door
   * \param $params the parameters array which contains : 
   * - id (INT >= 0) : the unique door id
   * - name (STRING) : the new door name
   * - idZ1 (INT >= 0) : the first location id
   * - idZ2 (INT >= 0) : the second location id 
   * \returns the new door parameters data in an array list that contains :
   * - id (INT >= 0) : the unique door id
   * - name (STRING) : the new name
   * - idZ1 (INT >= 0) : the new first location id
   * - idZ2 (INT >= 0) : the new second location id
   */
  function processDoorUpdate($params)
    {
      return $this->processFlowUpdate($params);
    }

  /**
   * \brief Inserts a new door.
   * 
   * Inserts a new Door and automatically add it to the door
   * counting processing table. Creates an unique id automatically.
   * \param $params the parameters array which contains : 
   * - name (STRING) : the new door name
   * - idZ1 (INT >= 0) the "from" location
   * - idZ2 (INT >= 0) the "to" location
   * \returns the new door parameters data in an array list @see processDoorUpdate
   */
  function processDoorInsert($params)
    {
      return $this->processFlowInsert($params);
    }

  /**
   * \brief Delete an existing Door.
   * 
   * Delete the existing Door.
   * \param $params the parameters array which contains : 
   * - id (INT >= 0) : an unique door id
   * \returns the reference of the deleted location in an array list 
   */
  function processDoorDelete($params)
    {
      return $this->processFlowDelete($params);
    }

  /**
   * \brief Returns the list of parameters of a specified group
   *
   * Returns the list of parameters of a specified Group including all the connected
   * doors parameters
   * \param $params the parameters array which contains : 
   * - id (INT >= 0) : the unique group id
   * \returns an array that contains : 
   * - id (INT >= 0) : the unique group id
   * - name (STRING) : the group name
   * - idZ1 (INT >= 0) : the first zone id
   * - idZ2 (INT >= 0) : the second zone id
   */
  function getGroupData($params)
    {      
      $res = $this->getFlows($params);     
      return $res[$params["id"]];
   }

  /**
   * Return the list of counting processing id for the door in the door group
   * with the following attributes
   * - idpD ; the counting processing of the door
   */
  function getGroupDoorChildCountingProcessing($params)
    {
      $query = "SELECT fcp.id AS idpD, f.name AS nameD " . 
	"FROM FlowCountingProcessing AS fcp, FlowFlowAssoc AS ffa, Flows AS f " .
	"WHERE fcp.flow = ffa.contained AND f.id = ffa.contained AND ffa.flow=" . $params["id"];
      return $this->get_config_array($query);
    }

  /**
   * \brief Insert the door in the door group in the FlowFlowAssoc table
   *
   * \param $params parameter array which contains
   * - id (INT >= 0) : the unique group id
   * - idZ1 (INT >= 0) : the first zone id
   * - idZ2 (INT >= 0) : the second zone id
   */
  function processGroupMemberInsert($params)
    {
      // add the door components
      $query = "SELECT id, name, 1 as sens FROM Flows WHERE  id != " . $params["id"] . " AND " .
	"zone1 IN (SELECT contained FROM ZoneZoneResolvedAssoc WHERE zone=" . $params["idZ1"] . ") AND " .
	"zone2 IN (SELECT contained FROM ZoneZoneResolvedAssoc WHERE zone=" . $params["idZ2"] . ") AND " .
	"zone2 NOT IN (SELECT contained FROM ZoneZoneResolvedAssoc WHERE zone=" . $params["idZ1"] . ") AND " .
	"zone1 NOT IN (SELECT contained FROM ZoneZoneResolvedAssoc WHERE zone=" . $params["idZ2"] . ") AND " .
	"NOT EXISTS(SELECT * FROM FlowFlowAssoc WHERE flow = Flows.id) union " . 
	"SELECT id, name, 2 as sens FROM Flows WHERE  id != " . $params["id"] . " AND " .
	"zone2 IN (SELECT contained FROM ZoneZoneResolvedAssoc WHERE zone=" . $params["idZ1"] . ") AND " .
	"zone1 IN (SELECT contained FROM ZoneZoneResolvedAssoc WHERE zone=" . $params["idZ2"] . ") AND " .
	"zone1 NOT IN (SELECT contained FROM ZoneZoneResolvedAssoc WHERE zone=" . $params["idZ1"] . ") AND " .
	"zone2 NOT IN (SELECT contained FROM ZoneZoneResolvedAssoc WHERE zone=" . $params["idZ2"] . ") AND " .
	"NOT EXISTS(SELECT * FROM FlowFlowAssoc WHERE flow = Flows.id)";

      $result = $this->configDB->query($query);
      while($obj = $result->nextAssoc())
	{
	  $query = "INSERT INTO FlowFlowAssoc(flow, contained, sens) VALUES(" . $params["id"] . "," . $obj["id"] . "," . $obj["sens"].");";
	  $res = $this->configDB->query($query);
	  $res->freeResult();
	}
      $result->freeResult();
    }

  /**
   * \brief Changes the parameters values of the specified group
   * 
   * Changes the parameters values of the specified group located in the door
   * group association.
   * \param $params the parameters array which contains : 
   * - id (INT >= 0) : the unique group id
   * - name (STRING) : the new group name
   * - doors (ARRAY OF INT PAIRS "INT_INT" ) : on the form doorId_sens
   * \returns the new parameters data in an array list @see getGroupData
   */
  function processGroupUpdate($params)
    {
      $retval = $this->processFlowUpdate($params);
    
      // delete all the doors associated with this group
      $query = "DELETE FROM FlowFlowAssoc WHERE flow=" . $params["id"];
      $result = $this->configDB->query($query);
      $result->freeResult();

      $this->processGroupMemberInsert($params);
   
      return $retval;
    }

  /**
   * \brief Inserts a new group.
   * 
   * Inserts a new Group and automatically add it to the group
   * counting processing table. Creates an unique id automatically.
   * Automatically add the doors that have the same location ids pair
   * into the door group association.
   * \param $params the parameters array which contains : 
   * - name (STRING) : the new door name
   * - doors (ARRAY OF INT PAIRS "INT_INT" ) : on the form doorId_sens
   * \returns the new group parameters data in an array list @see processGroupUpdate
   */
  function processGroupInsert($params)
    {
      $retval = $this->processFlowInsert($params);
      
      $params["id"] = $retval["id"];
      
      $this->processGroupMemberInsert($params);

      return $retval;
    }

  /**
   * \brief Delete an existing Group.
   * 
   * Delete the existing Group.
   * \param $params the parameters array which contains : 
   * - id (INT >= 0) : an unique group id
   * \returns the reference of the deleted group in an array list 
   */
  function processGroupDelete($params)
    {
      return $this->processFlowDelete($params);
    }

  /**
   * \brief Returns the list of parameters of a specified Area
   *
   * Returns the list of parameters of a specified Area one element
   * for each location component.
   * \param $params the parameters array which contains : 
   * - id (INT >= 0) : the unique area id
   * \returns a list where each element contains : 
   * - id (INT >= 0) : the unique area id
   * - name (STRING) : the area name
   * - idL (INT >= 0) : the location component id
   * - idpL (INT >= 0) : the location component processing id
   * - nameL (STRING) : the location component name
   */
  function getAreaData($params)
    {
      $query = "select Zones1.id as id, Zones1.name as name, ZoneZoneAssoc.contained as idL," . 
	"Zones2.name as nameL " .
	"from Zones As Zones1, Zones As Zones2, ZoneZoneAssoc " .
	"where Zones1.id=" . $params["id"] . " and Zones1.id = ZoneZoneAssoc.zone and " .
	"Zones2.id=ZoneZoneAssoc.contained";
      return $this->get_config_array($query);
    }

  /**
   * Return the list of counting processing id for the location in area
   * with the following attributes
   * - idpL : the counting processing of the location
   * - nameL : the name of the location
   */
  function getAreaChildCountingProcessing($params)
    {
      $query = "SELECT zcp.id AS idpL, z.name AS nameL FROM ".
	"ZoneCountingProcessing AS zcp, ZoneZoneAssoc AS zza, Zones AS z ".
	"WHERE zcp.zone = zza.contained AND z.id = zza.contained AND zza.zone=" . $params["id"];
      return $this->get_config_array($query);
    }

  function getAreaChildNumberingProcessing($params)
    {
      $query = "SELECT znp.id AS idpL, z.name AS nameL, z.id as idL FROM ".
	"ZoneNumberingProcessing AS znp, ZoneZoneAssoc AS zza, Zones AS z ".
	"WHERE znp.zone = zza.contained AND z.id = zza.contained AND zza.zone=" . $params["id"];
      return $this->get_config_array($query);
    }
  
  /**
   * \brief Changes the parameters values of the specified Area
   * 
   * Changes the parameters values of the specified Area
   * \param $params the parameters array which contains : 
   * - id (INT >= 0) : the unique id
   * - name (STRING) : the new name
   * - locations (ARRAY OF INT >= 0) : an array of all the new locations
   *   contained into this area
   * \returns the new parameters data in an array list @see getAreaData
   */
  function processAreaUpdate($params)
    {
      $this->processZoneUpdate($params);
  
      // delete previous location components
      $query = "DELETE FROM ZoneZoneAssoc WHERE zone=" . $params["id"];
      $result = $this->configDB->query($query);
      $result->freeResult();

      // insert new location components
      foreach($params["locations"] as $v)
	{
	  $query = "INSERT INTO ZoneZoneAssoc(zone, contained) VALUES(" . $params["id"] . ", $v)";
	  $result = $this->configDB->query($query);
	  $result->freeResult();
	}
      return array( array( $params["id"], $params["name"], $params["locations"] ) );
    }

  /**
   * \brief Inserts a new Area.
   * 
   * Inserts a new Area and automatically add it to the area
   * counting processing table. Creates an unique id automatically.
   * \param $params the parameters array which contains : 
   * - name (STRING) : the new area name
   * - locations (ARRAY OF INT >= 0) : an array of all the new locations
   *   contained into this area
   * \returns the new parameters data in an array list @see getAreaData
   */
  function processAreaInsert($params)
    {
      $retval = $this->processZoneInsert($params);

      $params["id"] =  $retval["id"];

      // add the sub area components
      foreach($params["locations"] as $v)
	{
	  $query = "insert into ZoneZoneAssoc(zone, contained) values(" . $params["id"] . ",$v)";
	  $result = $this->configDB->query($query);
	  $result->freeResult();
	}
   
      return $retval;
    }

  /**
   * \brief Delete an existing Area.
   * 
   * Delete the existing Area. Removes automatically the location components.
   * \param $params the parameters array which contains : 
   * - id (INT >= 0) : an unique location id
   * \returns the reference of the deleted area in an array list 
   */
  function processAreaDelete($params)
    {
      return $this->processZoneDelete($params);
    }

  /**
   * \private getWaitingTimeData
   */
  function getWaitingTimeData($params)
    {
      return $this->getZoneWaitingTimeProcessing($params);
    }

  /**
   * \private getProcessTimeData
   */
  function getProcessTimeData($params)
    {
      return $this->getZoneProcessTimeProcessing($params);
    }

  /**
   * 
   */
  function getWaitingTimeType()
  {
    return $this->get_config_array("select * from WaitingTimeType");
  }

  /**
   * 
   */
  function getProcessTimeType()
  {
    return $this->get_config_array("select * from ProcessTimeType");
  }


  /**
   * \private processWaitingTimeUpdate
   */
  function processWaitingTimeUpdate($params)
    {
      $query = "UPDATE ZoneWaitingTimeProcessing SET zone=" . $params["zoneid"] . "," .
	"flow=" . $params["flowid"] . "," .
	"sens=" . $params["sens"] . "," .
	"wt_id=" . $params["wt_id"] . " WHERE id=" . $params["id"];
      $result = $this->configDB->query($query);
      $result->freeResult();  	

      return array( array( $params["id"], $params["zoneid"], $params["flowid"], $params["sens"], $params["wt_id"] ) );
    }

  /**
   * \private processWaitingTimeInsert
   */
  function processWaitingTimeInsert($params)
    {
      // get a new area waiting time processing id
      $query = "SELECT IFNULL(MAX(id) + 1, 0) AS id FROM ZoneWaitingTimeProcessing";
      $result = $this->configDB->query($query);
      $obj = $result->nextAssoc();
      $result->freeResult();  
      $params["id"] = $obj["id"];

      $query = "INSERT INTO ZoneWaitingTimeProcessing(id, zone, flow, sens, calendar, wt_id) SELECT " . 
	$params["id"] . "," . $params["zoneid"] . "," . $params["flowid"] . "," . $params["sens"] . ", id, " . $params["wt_id"] . " FROM Calendars LIMIT 1";
      $result = $this->configDB->query($query);
      $result->freeResult();  

      return array( array( $params["id"], $params["zoneid"], $params["flowid"], $params["sens"] ) );
    }

  /**
   * \private processWaitingTimeDelete
   */
  function processWaitingTimeDelete($params)
    {
      $query = "DELETE FROM ZoneWaitingTimeProcessing WHERE id=" . $params["id"];
      $result = $this->configDB->query($query);
      $result->freeResult();  

      return array( array( $params["id"] ) );
    }

  /**
   * \private processWaitingTimeUpdate
   */
  function processProcessTimeUpdate($params)
    {
      $query = "UPDATE ZoneProcessTimeProcessing SET zone=" . $params["zoneid"] . "," .
	"flow=" . $params["flowid"] . "," .
	"sens=" . $params["sens"] . "," .
	"pt_id=" . $params["pt_id"] . " WHERE id=" . $params["id"];
      $result = $this->configDB->query($query);
      $result->freeResult();  	
      return array( array( $params["id"], $params["zoneid"], $params["flowid"], $params["sens"], $params["pt_id"] ) );
    }

  /**
   * \private processWaitingTimeInsert
   */
  function processProcessTimeInsert($params)
    {
      // get a new area waiting time processing id
      $query = "SELECT IFNULL(MAX(id) + 1, 0) AS id FROM ZoneProcessTimeProcessing";
      $result = $this->configDB->query($query);
      $obj = $result->nextAssoc();
      $result->freeResult();  
      $params["id"] = $obj["id"];
      $query = "INSERT INTO ZoneProcessTimeProcessing(id, zone, flow, sens, calendar, pt_id) SELECT " . 
	$params["id"] . "," . $params["zoneid"] . "," . $params["flowid"] . "," . $params["sens"] . ", id, " . $params["pt_id"] . " FROM Calendars LIMIT 1";
      $result = $this->configDB->query($query);
      $result->freeResult();  
      return array( array( $params["id"], $params["zoneid"], $params["flowid"], $params["sens"] ) );
    }

  /**
   * \private processWaitingTimeDelete
   */
  function processProcessTimeDelete($params)
    {
      $query = "DELETE FROM ZoneProcessTimeProcessing WHERE id=" . $params["id"];
      $result = $this->configDB->query($query);
      $result->freeResult();  
      return array( array( $params["id"] ) );
    }

  /**
   * \private getProcessTimeValues
   */
  function getProcessTimeValues($params)
  {
    if($this->configDB->isTableExist("ProcessTimeType"))
      {
	$where = "where pt_id=ProcessTimeType.id";	
	if(isset($params["idp"]))
	  {
	    $where .= " and ZoneProcessTimeProcessing.id=" . $params["idp"];
	  }
	$query = "SELECT name, ZoneProcessTimeProcessing.id as idp FROM ProcessTimeType, ZoneProcessTimeProcessing $where";
	$result = $this->configDB->query($query);
	$values = array();
	while($obj = $result->nextAssoc())
	  {
	    $params["idp"] = $obj["idp"];
	    $pt = $this->getProcessingClass($obj["name"], $params);
	    $data = $pt->process($params);
	    foreach($data as $k => $v)
	      {
		$values[] = $v;
	      }
	  }
	$result->freeResult();
	return $values;
      }
    $params["valueKind"] = "processTime";
    return $this->getValues($params);
  }
  
  /**
   * \private getWaitingValues
   */
  function getWaitingTimeValues($params)
    {
      if($this->configDB->isTableExist("WaitingTimeType"))
	{
	  $extraTable = "";
	  $where = "where wt_id=WaitingTimeType.id";
	  if(isset($params["idp"]))
	    {
	      $where .= " and ZoneWaitingTimeProcessing.id=" . $params["idp"];
	    }
	  if(isset($params["withExport"]) and $params["withExport"])
	    {
	      $where .= " and indicator=\"ZoneWaitingTimeProcessing\" and module=\"" . 
		$params["withExportModule"] . "\" and doExport=1 " .
		"and Export.id=ZoneWaitingTimeProcessing.id";
	      $extraTable = ",Export";
	    }
	  $query = "SELECT name, ZoneWaitingTimeProcessing.id as idp FROM WaitingTimeType, ZoneWaitingTimeProcessing" . $extraTable . " $where order by idp asc";
	  $result = $this->configDB->query($query);
	  $values = array();
	  while($obj = $result->nextAssoc())
	    {
	      $params["idp"] = $obj["idp"];
	      $wt = $this->getProcessingClass($obj["name"], $params);
	      $data = $wt->getValuesInDB($params);
	      foreach($data as $k => $v)
		{
		  $values[] = $v;
		}
	    }
	  $result->freeResult();
	  return $values;
	}
      $params["valueKind"] = "waitingTime";
      return $this->getValues($params);
    }

  /**
   * \brief Returns bi-directional counting flow information for time parameters
   * 
   * Returns bi-directional counting flow information for time parameters.
   * \param $params the array parameters which contains :
   * - idp (INT >= 0) : the unique processing id
   * - Entity ('location' | 'door' | 'group' | 'area' ) : object type
   * - [TimeStamp (INT >= 0)] : Unix time format
   * - [TimeStr (STRING)] : Timestamp in the format YYYYMMDDHHMMSS
   * - Step ('minute', 'hour', 'day', 'month', 'year') : cumulative interval for counting values
   * - CompPeriod ('hour', 'day', 'month', 'year') : time period to compute the mean value
   * - optional withExport
   * The TimeStamp parameter is optional, you can use the TimeStr instead. The CompPeriod must
   * be greater than the Step value.
   * \returns bi-directional counting flow information for time parameters.
   * - idp (INT >= 0) : the unique processing id
   * - value0 (FLOAT >= 0) : the mean in flow value / Step on CompPeriod time for the TimeStamp date
   * - value1 (FLOAT >= 0) : the mean out flow value / Step on CompPeriod time for the TimeStamp date
   * - period (STRING) : the step period time
   */
  function getCountersValues($params, $useRawData = false) 
    {
      if(!$useRawData and $this->configDB->isTableExist("FlowCountingType"))
	{
	  $extraTable = "";
	  $where = "where fc_id=FlowCountingType.id";
	  if(isset($params["idp"]))
	    {
	      $where .= " and FlowCountingProcessing.id=" . $params["idp"];
	    }
	  if(isset($params["withExport"]) and $params["withExport"])
	    {
	      $where .= " and indicator=\"FlowCountingProcessing\" and module=\"" . 
		$params["withExportModule"] . "\" and doExport=1 " . 
		"and Export.id=FlowCountingProcessing.id";
	      $extraTable = ",Export";
	    }
	  $query = "select name, FlowCountingProcessing.id as idp from FlowCountingType, FlowCountingProcessing" . $extraTable . " $where order by idp asc";
	  
	  $result = $this->configDB->query($query);
	  $values = array();
	  while($obj = $result->nextAssoc())
	    {
	      $params["idp"] = $obj["idp"];
	      $zc = $this->getProcessingClass($obj["name"], $params);
	      $data = $zc->process($params);
	      if($data)
		{
		  foreach($data as $k => $v)
		    {
		      $values[] = $v;
		    }
		}
	    }
	  $result->freeResult();
	  return $values;
	}
      $params["valueKind"] = "counting";
      return $this->getValues($params);
    }

  function getVisitorsValues($params) 
  {
    if($this->configDB->isTableExist("ZoneCountingType"))
      {
	$where = "where zc_id=ZoneCountingType.id";
	if(isset($params["idp"]))
	  {
	    $where .= " and ZoneCountingProcessing.id=" . $params["idp"];
	  }
	$query = "select name, ZoneCountingProcessing.id as idp from ZoneCountingType, ZoneCountingProcessing $where";

	$result = $this->configDB->query($query);

	$values = array();
	while($obj = $result->nextAssoc())
	  {
	    $params["idp"] = $obj["idp"];
	    $zc = $this->getProcessingClass($obj["name"], $params);
	    $data = $zc->process($params);
	    if($data)
	      {
		foreach($data as $k => $v)
		  {
		    $values[] = $v;
		  }
	      }
	  }
	$result->freeResult();
	return $values;
      }
    $params["valueKind"] = "counting";
    return $this->getValues($params);
  }


 /**
   * \brief Returns value (bi-directional counting flow information / numbering) for time parameters
   * 
   * Returns bi-directional counting flow information for time parameters or numbering.
   * \param $params the array parameters which contains :
   * - valueKind : "counting" | "numbering" | waitingTime
   * - idp (INT >= 0) : the unique processing id
   * - Entity ('location' | 'door' | 'group' | 'area' ) : object type
   * - [TimeStamp (INT >= 0)] : Unix time format
   * - [TimeStr (STRING)] : Timestamp in the format YYYYMMDDHHMMSS
   * - Step ('minute', 'hour', 'day', 'month', 'year') : cumulative interval for counting values
   * - CompPeriod ('hour', 'day', 'month', 'year') : time period to compute the mean value
   * The TimeStamp parameter is optional, you can use the TimeStr instead. The CompPeriod must
   * be greater than the Step value.
   * \returns bi-directional counting flow information for time parameters.
   * - idp (INT >= 0) : the unique processing id
   * if counting 
   *   - value0 (FLOAT >= 0) : the mean in flow value / Step on CompPeriod time for the TimeStamp date
   *   - value1 (FLOAT >= 0) : the mean out flow value / Step on CompPeriod time for the TimeStamp date
   * if numbering, waitingTime
   *   - value (FLOAT >= 0) : the mean in numbering or waiting timevalue / Step on CompPeriod time for the TimeStamp date
   * - period (STRING) : the step period time
   * - periodInt (INT) : the step period as int for comparison
   */
  function getValues($params)
    {
      if(!isset($params["valueKind"])) return false;

      if($params["Entity"] == "location" || $params["Entity"] == "area")
	{
	  if($params["valueKind"] == "counting") $ctable = "ZoneCounting";
	  else if($params["valueKind"] == "numbering") $ctable = "ZoneNumbering";
	  else if($params["valueKind"] == "waitingTime") $ctable = "ZoneWaitingTime";
	  else return false;
	}
      else if($params["Entity"] == "door" || $params["Entity"] == "group")
	{
	  if($params["valueKind"] == "counting") $ctable = "FlowCounting";
	  else return false;
	}
      else
	{
	  return false;
	}
      
      if($params["Step"] == "day" || $params["Step"] == "month" || $params["Step"] == "year")
	{
	  $ctable .= "_DAY";
	}
      else 
	{
	  $ctable .= "_MINUTE";
	}
      
      if(!isset($params["TimeStamp"]))
	{
	  if(isset($params["TimeStr"]))
	    {
	      $params["TimeStamp"] = mktimeFromString($params["TimeStr"]);
	    }
	  else
	    {
	      // FAB : must be UTC
	      $params["TimeStamp"] = time();
	    }
	}

      if(!isset($params["idp"]))
	{
	  $params["idp"] = null;
	}

      return $this->get_table_values($params["valueKind"], $params["Step"], $params["idp"], $params["CompPeriod"], $params["TimeStamp"], $ctable);
    }

  function getNumberingValues($params)
    {
      if($this->configDB->isTableExist("NumberingType"))
	{
	  $extraTable = "";
	  $where = "where numb_id=NumberingType.id";
	  if(isset($params["idp"]))
	    {
	      $where .= " and ZoneNumberingProcessing.id=" . $params["idp"];
	    }
	  if(isset($params["withExport"]) and $params["withExport"])
	    {
	      $where .= " and indicator=\"ZoneNumberingProcessing\" and module=\"" . 
		$params["withExportModule"] . "\" and doExport=1 " .
		"and Export.id=ZoneNumberingProcessing.id";
	      $extraTable = ",Export";
	    }
	  $query = "SELECT name, ZoneNumberingProcessing.id as idp FROM NumberingType, ZoneNumberingProcessing" . $extraTable . " $where order by idp asc";

	  $result = $this->configDB->query($query);

	  $values = array();
	  while($obj = $result->nextAssoc())
	    {
	      $params["idp"] = $obj["idp"];
	      $wt = $this->getProcessingClass($obj["name"], $params);
	      $data = $wt->getValuesInDB($params); 
	      foreach($data as $k => $v)
		{
		  $values[] = $v;
		}
	    }
	  $result->freeResult();
	  return $values;
	}
      $params["valueKind"] = "numbering";
      return $this->getValues($params);
    }

  function getZoneNumberingValues($params)
  {
    $params["valueKind"] = "numbering";
    return $this->getValues($params);  
  }

  /**
   * \brief Returns the specified object parameters
   * 
   * Returns the specified object parameters.
   * \param $params the array parameters which contains :
   * - Entity ('group' | 'area' ) : Entity type
   * - id (INT >= 0) : Entity unique id
   */
  function getCountingObjectsIdx($params)
    {
      if($params["Entity"] == "group")
	{
	  return $this->getGroupDoorChildCountingProcessing($params);
	}
      else if($params["Entity"] == "area")
	{
	  return $this->getAreaChildCountingProcessing($params);
	}      
    }

  function getNumberingObjectsIdx($params)
    {
      if($params["Entity"] == "area")
	{
	  return $this->getAreaChildNumberingProcessing($params);
	}  
    }

  function getFlowCountingProcessingParams($params)
  {
    return $this->getProcessingParams("FlowCounting", $params);
  }

  function getProcessingClass($basename, $params)
  {
    $userClass = sprintf("${basename}_%s", $params["idp"]);
    $userDefinedClass = BASEPATH . "/BTopLocalServer/indicators/" . $userClass . ".inc";
    $systemDefinedClass = "BluePHP/BTopLocalServer/indicators/" . $basename . ".inc";
    $fileClass = $systemDefinedClass;
    if(file_exists($userDefinedClass))
      {
	$fileClass = $userDefinedClass;
	$basename = $userClass;
      }
    include_once($fileClass);
    return new $basename($this);
  }

  /**
   * \brief Returns the indicator parameters
   */
  function getProcessingParams($basename, $params)
  {    
    $fileexp = "${basename}_%s.inc";
    $basepath = BASEPATH . "BTopLocalServer/indicators/";
    $filename = sprintf($fileexp, $params["idp"]);

    if(!file_exists($basepath . $filename))
      {
	$filename = sprintf($fileexp, "default");
      }
    include_once($basepath . $filename);
    $obj = new $basename();
    return $obj->getProcessingParams($params);
  }

  /**
   * \brief Returns the list of rules from the specified calendar
   * 
   * Returns the list of rules from the specified calendar sorted by increasing
   * priority.
   * \param $params the array parameters which contains : 
   * - id (INT >= 0) : The unique calendar id
   * \return the list of rules in an array which contains : 
   * - id (INT >= 0) : The unique rule id
   * - cal (INT >= 0) : The unique calendar id
   * - pri (INT >= 0) : The rule priority
   * - testts (STRING for strftime) : The strftime rule definition for period starting
   * - ts (TIMESTAMP) : The period start
   * - testte (STRING for strftime) : The strftime rule definition for period ending
   * - te (TIMESTAMP) : The period end
   * - isOpen (0|1) : 0 means that the rule must not match, 1 means that the rule must match
   */
  function getCalendarRules($params)
    {
      $query = "select id, cal, pri, testts, ts, testte, te, isOpen from CalData where cal=" . $params["id"] . " order by pri asc";
      return $this->get_config_array($query);
    }

  /**
   * \brief Returns the parameters of a specified Calendar rule
   * 
   * Returns the parameters of a specified Calendar rule.
   * \param $params the parameters array which contains :
   * - id (INT >= 0) : the unique calendar rule
   * \return The the function returns an array that contains @see getCalendarRules
   */
  function getCalendarData($params)
    {
      $query = "select id, cal, pri, testts, ts, testte, te, isOpen from CalData where id=" . $params["id"];
      return $this->get_config_array($query);      
    }

  /**
   * \brief Changes the parameters value of the specified calendar rule
   *
   * Change the parameters values of the specified calendar rule.
   * \param $params the parameters array which contains : 
   * - id (INT >= 0) : the unique calendar rule id
   * - cal (INT >= 0) : the calendar id
   * - pri (INT >= 0) : the rule priority
   * - testts (STRING for strftime) : The strftime rule definition for period starting
   * - ts (TIMESTAMP) : The period start
   * - testte (STRING for strftime) : The strftime rule definition for period ending
   * - te (TIMESTAMP) : The period end
   * - isOpen (0|1) : 0 means that the rule must not match, 1 means that the rule must match
   */
  function processCalendarUpdate($params)
    {
      $query = "update CalData set cal=" . $params["cal"] . 
	",pri=" . $params["pri"] . 
	",testts=\"" . $params["testts"] . "\"" .
	",ts=\"" . $params["ts"] . "\"" . 
	",testte=\"" . $params["testte"] . "\"" .  
	",te=\"" . $params["te"] . "\"" . 
	",isOpen=" . $params["isOpen"] . 
	" where id=" . $params["id"];
      $result = $this->configDB->query($query);
      $result->freeResult();

      return array( $params );
    }

  /**
   * \brief Insert a new calendar rule
   * 
   * Creates a new calendar rule.
   * \param $params the parameters array which contains : 
   * - cal (INT >= 0) : the calendar id
   * - pri (INT >= 0) : the rule priority
   * - testts (STRING for strftime) : The strftime rule definition for period starting
   * - ts (TIMESTAMP) : The period start
   * - testte (STRING for strftime) : The strftime rule definition for period ending
   * - te (TIMESTAMP) : The period end
   * - isOpen (0|1) : 0 means that the rule must not match, 1 means that the rule must match
   * \return the new rule parameters @see getCalendarData
   */
  function processCalendarInsert($params)
    {
      $query = "insert into CalData(cal,pri,testts,ts,testte,te,isOpen) values(" .
	$params["cal"] . "," . $params["pri"] . ",\"" . $params["testts"] . "\",\"" . $params["ts"] . "\"" . 
	",\"" . $params["testte"] . "\",\"" . $params["te"] . "\"," . $params["isOpen"] . ")";
      $result = $this->configDB->query($query);
      $result->freeResult();

      return array( $params );
    }

  /**
   * \brief Delete an existing calendar rule.
   * 
   * Delete an existing calendar rule.
   * \param $params the parameters array which contains : 
   * - id (INT >= 0) : an unique calendar rule
   * \returns the id parameters 
   */
  function processCalendarDelete($params)
    {
      $query = "delete from CalData where id=" . $params["id"];

      $result = $this->configDB->query($query);
      $result->freeResult();
      
      return array( array( $params["id"] ) );
    }

  /**
   * \brief Returns the timezone parameter
   * 
   * Returns the timezone parameter
   * \param $params the parameters array not used
   * \return The the function returns an array that contains the timezone parameter
   */
  function getTimeZoneData($params)
    {
      $query = "select tz from TimeZone";
      return $this->get_config_array($query);      
    }

  /**
   * \brief Changes the timezone parameter
   *
   * \param $params the parameters array that contains
   * - tz (STRING) : the timezone identifier @see timezone_identifiers_list
   * \return the given parameters
   */
  function processTimeZoneUpdate($params)
    {
      $query = "update TimeZone set tz=\"" . $params["tz"] . "\"";
      $result = $this->configDB->query($query);
      $result->freeResult();
      return array( $params );
    }

  /**
   * \brief Convert a timestring YYYYMMDDHHMMSS in UTC to
   * a timestring YYYY-MM-DD HH:MM:SS in TimeZone
   * 
   */
  function convertToTimeZone($timestamp)
    {
      $dateTime = new DateTime( substr($timestamp, 0, 4) . "-" . 
				substr($timestamp, 4, 2) . "-" . 
				substr($timestamp, 6, 2) . " " .
				substr($timestamp, 8, 2) . ":" .
				substr($timestamp, 10, 2) . ":" .
				substr($timestamp, 12, 2), new DateTimeZone("UTC") );
      $res = $this->getTimeZoneData( array() );
      $dateTime->setTimeZone(new DateTimeZone($res[0]["tz"]));
      return $dateTime->format("Y-m-d H:i:s");
    }

  function get_period_values($params)
    {
      // get meta informations from the step
      $periodInfos = new PeriodMetaData($params["step"]);

      if($params["step"] != "minute" and $params["step"] != "hour")
	{
	  // simple => BCL.db
	  if($this->accessDataDB($params["step"], $params["timeStart"]))
	    {
	      //	      $sqlQuery = new 
	    }
	  
	}
      else
	{
	  // cas plus compliqué avec boucle par jour
	  $oneDay = 60*60*24;
	  for($cur = $timeStart ; $cur <= $timeEnd ; $cur += $oneDay)
	    {
	      // BCL_cur
	      if($this->accessDataDB($params["step"], $params["timeStart"]))
		{
		  // concat data
		}
	    }
	}
    }

  /**
   * \private get_table_values
   *
   * nstep : 
   * [number]minute ou [number]hour [number]day [number]month [number]year
   * [number] is an optional number that is a factor, example 5minute mean data each five minutes
   *
   * \param type : counting / numbering
   */
  function get_table_values($type, $nstep, $id, $granularity, $timestamp, $ctable)
    {
      $n = 1;
      $number = "";
      $idx = 0;
      while(ctype_digit($nstep[$idx]))
	{
	  $number .= $nstep[$idx];
	  $idx++;
	}
      if($number != "") $n = $number;
      $step=substr($nstep, $idx);


      if($step == "minute")
	{
	  $periodStr = "%Mmin";
          $periodIntStr = "%M";
	  $groupDistinctionStr = "%Y%m%d%H";
	  $groupStr = "%M";
	}
      else if($step == "hour")
	{
	  $periodStr = "%Hh";
          $periodIntStr = "%H";
	  $groupDistinctionStr = "%Y%m%d";
	  $groupStr = "%H";
	}
      else if($step == "day")
	{
	  $periodStr = "%d";
          $periodIntStr = "%d";
	  $groupDistinctionStr = "%Y%m";
	  $groupStr = "%d";
	}
      else if($step == "month")
	{
	  $periodStr = "%m";
          $periodIntStr = "%m"; 
	  $groupDistinctionStr = "%Y";
	  $groupStr = "%m";
	}
      else if($step == "year")
	{
	  $periodStr = "%Y";
          $periodIntStr = "%Y"; 
	  $groupDistinctionStr = "";
	  $groupStr = "%Y";
	}
      else
	{
	  return false;
	}
      $dateSel = $this->get_sql_where_for_granularity($granularity, $timestamp);

      if($dateSel)
	{
	  if($type == "counting")
	    {
	      return $this->get_counting_values($step, $timestamp,
						$id, 
						$dateSel, 
						$n, 
						$periodStr,
                                                $periodIntStr,
						$groupStr,
						$groupDistinctionStr,
						$ctable);
	    }
	  else if($type == "numbering"  || $type = "waitingTime")
	    {
	      /*return $this->get_mono_values($step, $timestamp,
					    $id, 
					    $dateSel, 
					    $n, 
					    $periodStr,
					    $periodIntStr,
					    $groupStr, $groupDistinctionStr,
					    $ctable, "AVG");*/
	      return $this->get_mean_values($step, $timestamp,
					    $id, 
					    $dateSel, 
					    $n, 
					    $periodStr,
					    $periodIntStr,
					    $groupStr, $groupDistinctionStr,
					    $ctable);
	    }
	  else return false;
	}
      return false;
    }

  /**
   * \private get_sql_where_for_granularity
   */
  function get_sql_where_for_granularity($granularity, $timestamp)
    {
      $dateSel = false;
      if($granularity == "minute")
	{
	  $date = strftime("%Y-%m-%d %H:%M", $timestamp);
	  $dateSel = "strftime('%Y-%m-%d %H:%M', start) = \"$date\"";
	}
      else if($granularity == "hour")
	{
	  $date = strftime("%Y-%m-%d %H", $timestamp);
	  $dateSel = "strftime('%Y-%m-%d %H', start) = \"$date\"";
	}
      else if($granularity == "day")
	{
	  $date = strftime("%Y-%m-%d", $timestamp);
	  $dateSel = "strftime('%Y-%m-%d', start) = \"$date\"";
	}
      else if($granularity == "month")
	{
	  $date = strftime("%Y-%m", $timestamp);
	  $dateSel = "strftime('%Y-%m', start) = \"$date\"";
	}
      else if($granularity == "year")
	{
	  $date = strftime("%Y", $timestamp);
	  $dateSel = "strftime('%Y', start) = \"$date\"";
	}
      return $dateSel;
    }

  /**
   * \private get_counting_values
   */
  function get_counting_values($step, $timestamp, $id, $dateSel, $n, $periodStr, $periodIntStr, $groupStr, $groupDistinctionStr, $ctable)
    {
      $data = array();
      $groupBy = "group by id, ";
      if($groupDistinctionStr != "")
	{
	  $groupBy .= "strftime('$groupDistinctionStr', start), ";
	}
      $groupBy .= "max(0, round((strftime('$groupStr', start) / $n) - 0.5))";

      $where = "where ";
      if($id != null)
	{
	  $where .= "id=$id and ";
	}
      $where .= $dateSel;

      $query = "select id, sum(value0) as value0," .
	" sum(value1) as value1," .
	" strftime('$periodStr', min(start)) as period," .
	" 0 + strftime('$periodIntStr', min(start)) as periodInt, min(start) as start, " .
	" max(start) as end, count(value0) as N from $ctable $where $groupBy";

      if($this->accessDataDB($step, $timestamp))
	{
	  $result = $this->dataDB->query($query);	
	  $data = $this->get_associative_array($result);
	  $result->freeResult();
	}
      return $data;
    }

  
  /**
   * \private get_mono_values
   */
  function get_mono_values($step, $timestamp, $id, $dateSel, $n, $periodStr, $periodIntStr, $groupStr, $groupDistinctionStr, $ctable, $op)
    {
      $groupBy = "group by id, ";
      if($groupDistinctionStr != "")
	{
	  $groupBy .= "strftime('$groupDistinctionStr', start), ";
	}
      $groupBy .= "max(0, round((strftime('$groupStr', start) / $n) - 0.5))";
      
      $query = "select id, $op(value) as value," .
	" strftime('$periodStr', min(start)) as period," . 
	" 0 + strftime('$periodIntStr', min(start)) as periodInt, min(start) as start, max(start) as end from $ctable where" .
	" id=$id and $dateSel $groupBy";

      if($this->accessDataDB($step, $timestamp))
	{
	  $result = $this->dataDB->query($query);	
	  $data = $this->get_associative_array($result);
	  $result->freeResult();
	}
      return $data;
    }

  /**
   * \private get_mono_values
   */
  function get_mean_values($step, $timestamp, $id, $dateSel, $n, $periodStr, $periodIntStr, $groupStr, $groupDistinctionStr, $ctable)
    {
      $data = array();
      $groupBy = "group by id, ";
      if($groupDistinctionStr != "")
	{
	  $groupBy .= "strftime('$groupDistinctionStr', start), ";
	}
      $groupBy .= "max(0, round((strftime('$groupStr', start) / $n) - 0.5))";

      $where = "where ";
      if($id != null)
	{
	  $where .= "id=$id and ";
	}
      $where .= $dateSel;
      
      $query = "select id, AVG(value) as value," .
	" ((sum(value*value) - AVG(value) * AVG(value)) / count(*)) as variance ," .
	" MAX(value) as maxvalue, MIN(value) as minvalue," .
	" strftime('$periodStr', min(start)) as period," . 
	" 0 + strftime('$periodIntStr', min(start)) as periodInt, " .
	"min(start) as start, max(start) as end from $ctable $where $groupBy";

      if($this->accessDataDB($step, $timestamp))
	{
	  $result = $this->dataDB->query($query);	
	  $data = $this->get_associative_array($result);
	  $result->freeResult();
	}
      return $data;
    }

//  function get_mean_values2($params)
//  {
//    $data = array();
//    $groupBy = "group by ";
//    if(isset($params["groupDistinctionStr"]))
//      {
//	$groupBy .= "strftime('" . $params["groupDistinctionStr"] . "', start), ";
//      }
//    $groupBy .= "max(0, round((strftime('" . $params["groupStr"] . "', start / " . $params["n"] . ") - 0.5))";
//
//    $where = "where ";
//    if(isset($params["id"]))
//      {
//	$where .= "id=" . $params["id"] . " and ";
//      }
//    $where .= $params["dateSel"];
//
//    $query = "select id, AVG(value) as value," . 
//      " ((sum(value*value) - AVG(value) * AVG(value)) / count(*)) as variance ," .
//      " MAX(value) as maxvalue, MIN(value) as minvalue," .
//      " strftime('" . $params["periodStr"] . "', min(start)) as period," . 
//      " 0 + strftime('" . $params["periodIntStr"] . "', min(start)) as periodInt," .
//      " min(start) as start, max(start) as end from " . $params["ctable"] . " " .
//      $where . " " . $params["groupBy"];
//
//      if($this->accessDataDB($step, $timestamp))
//	{
//	  $result = $this->dataDB->query($query);	
//	  $data = $this->get_associative_array($result);
//	  $result->freeResult();
//	}
//      return $data;      
//  }

  /**
   * \private get_idx_associative_array
   */
  function get_idx_associative_array($query)
    {
      $result = $this->configDB->query($query);
      $data = array();
      while($obj = $result->nextAssoc())
	{
	  $data[$obj["idx"]] = $obj;
	}
      $result->freeResult();
      return $data;
    }

  /**
   * \private get_associative_array
   * 
   * Returns the associative array
   * containing all the results rows
   */
  function get_associative_array($result, $idx = false)
    {
      $data = array();
      while($obj = $result->nextAssoc())
	{
	  if($idx)
	    {
	      $data[$obj[$idx]] = $obj;
	    }
	  else
	    {
	      $data[] = $obj;
	    }
	}
      return $data;      
    }

  /**
   * \private get_config_array
   * 
   * Make a query for the config
   * database, store the results into
   * an associative array, free the results
   * and return the array
   */
  function get_config_array($query, $idx = false)
    {
      //      var_dump($query);
      //      var_dump($idx);
      $result = $this->configDB->query($query);
      $data = $this->get_associative_array($result, $idx);
      $result->freeResult();
      return $data;
    }

  /**
   * \private get_number_of_entries
   *
   * Returns the number of entries in a table.
   */
  function get_number_of_entries($tableName, $where = "")
    {
      $nb = 0;
      $query = "select count(*) as Nb from $tableName " . $where;
      $result = $this->configDB->query($query);
      $obj = $result->nextAssoc();
      if($obj)
	{
	  $nb = $obj["Nb"];
	}
      $result->freeResult();
      return $nb;
    }

  /**
   * \private remove all the entries in the specified table
   */
  function clear_table(&$db, $tableName)
    {
      $query = "delete from $tableName";
      $result = $db->query($query);
      $result->freeResult();
    }

  /**
   * \brief Close the connection
   */
  function close()
    {
      $this->configDB->closeDB();

      if($this->dataDB)
	{
	  $this->dataDB->closeDB();
	}

      if($this->countDB)
	{
	  $this->countDB->closeDB();
	}
    }
}
/** @} */
?>
