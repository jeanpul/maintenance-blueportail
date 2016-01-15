<?php

include_once("BluePHP/Utils/DBConnect.inc");

/**
 * \file ConfigDB.inc
 * \brief Interface to the configuration data
 * TODO (FAB)
 * - change database column names (uses owner instead of login, uses key instead of name)
 * - add Owner table instead of using User table 
 * - add SectionsList table that contains the list of used sections (the list of table name)
 */

/**
 * \class ConfigDB
 * \brief Interface to the configuration data
 *
 * A configuration object is a set of (key, value) pairs divided into
 * sections which can be either owned by a specific user (the owner) or
 * used by everybody (shared).
 * Internal structure uses a SQL database with 2 tables per sections to store
 * every data. One table that store the data for all owners in the format
 * - (name [STRING], value [STRING], access [INT])
 * access could be used to enable or disable some parameters for specific users.
 * and the other that store the data per owner :
 * - (login [STRING], name [STRING], value [STRING])
 * Sections list is stored into a specific table where only the name of the shared data table is used. Then
 * the user data table name is build by add the string "UserAssoc" to the table name.
 */
/** \addtogroup Session */
/** @{ */
class ConfigDB
{
  /**
   * \private dataDB
   * Connection to the config SQL database
   */
  var $configDB;


  /**
   * \brief Create the Config interface
   */
  public function __construct($dbparams = false)
  {
    if($dbparams)
      {
	$this->openDB($dbparams);
      }
    else
      {
	$this->openDBOld();
      }
  }

  function openDBOld()
  {
    $this->configDB = new DBConnect(SESSION_DBTYPE, SESSION_DBHOST, SESSION_DBNAME,
				    SESSION_DBUSER, SESSION_DBPASSWD);
    $this->configDB->connectToDB();
  }

  function openDB($params)
  {
    $this->configDB = new DBConnect($params["TYPE"], $params["HOST"], $params["NAME"], $params["USER"], $params["PASSWD"]);
    $this->configDB->connectToDB();    
  }

  /**
   * \brief Returns the configuration data
   *
   * Returns an array containing all the configuration data for the specified session.
   * The array is indexed by the parameter name and if on the form :
   * ( "name1" => ( "login", "name1", "value1" ), "name2" => ( "login", "name2", "value2"), ... )
   * \param the session parameter array containing the login and the access level
   */
  function getConfigData($session, $configTable = "Config", $userTableName = "User")
    {
      $configTableAssoc = $configTable . "UserAssoc";
      $query = "select ifnull($configTableAssoc.login, U.login) as login, U.access as access, " . 
	"$configTable.name as name," .
	"ifnull($configTableAssoc.value, $configTable.value) as value from $configTable, $userTableName as U " . 
	"left join $configTableAssoc on $configTableAssoc.name = $configTable.name and $configTableAssoc.login = U.login " . 
	"where U.login='" . $session["login"] . "' and $configTable.access <= " . $session["access"];
      return $this->get_config_idx_data("name", $query);
    }

  /**
   * getConfigData for BluePortail
   */
  function getConfigDataBP($session)
    {
      return $this->getConfigData($session, "Config", "Client");
    }

  /**
   * \brief Returns the shared configuration data
   */
  function getSharedConfigData($section = "Config")
    {
      $query = "select * from $section";
      return $this->get_config_idx_data("name", $query);
    }

  /**
   * \brief Add a new entry in a configuration table
   */
  function addSharedConfigData($key, $value, $access, $section = "Config")
    {
      $query = "insert into $section values('$key', '$value', $access)";
      $result = $this->configDB->query($query);
      $result->freeResult();
    }

  /**
   * \brief Change the configuration data
   *
   * \param session : the session parameters
   * \param the configuration parameter array in the form 
   * ( "name1" => "value1" ), "name2" => "value2"), ... )
   */
  function updateConfigData($session, $data, $table = "Config")
    {
      $tableAssoc = $table . "UserAssoc";

      foreach($data as $k => $v)
	{
	  if($this->havePermission($session, $k, $table))
	    {
	      if($this->haveConfigValue($session, $k, $tableAssoc))
		{
		  $query = "update $tableAssoc set value='" . $v . "' " .
		    " where login='" . $session["login"] . "' and name='" . $k . "'";
		}
	      else
		{
		  $query = "insert into $tableAssoc values(" .
		    "'" . $session["login"] . "','" . $k . "','" . $v . "')";
		}
	    }
	  $result = $this->configDB->query($query);
	  $result->freeResult();
	}
    }

  /**
   * \brief Change the shared data
   *
   * \param session : the session parameters
   * \param the configuration parameter array in the form
   * ( "name1" => ( "name1", "value1", "access1" ), "name2" => ( "name2", "value2", "access2" ), ... )
   */
  function updateSharedConfigData($session, $data)
    {
      foreach($data as $k => $v)
	{
	  $this->updateSharedConfigValue($session, $v["name"], $v["value"]);
	}
    }

  /**
   * \brief Change a specific shared parameter value
   *
   * \param session : the session parameters
   * \param name (STRING) : the parameter name
   * \param value (STRING) : the value 
   */
  function updateSharedConfigValue($session, $name, $value)
    {
      // check if the user have enough right to modify this parameter
      if($this->havePermission($session, $name))
	{
	  $query = "update Config set value='$value' where name='$name'";
	  $result = $this->configDB->query($query);
	  $result->freeResult();
	}
    }

  /**
   * \brief Returns true if the configuration parameter exist
   * 
   * \param session the session parameters
   * \param id (INTEGER) : the configuration parameter unique id
   */
  function haveConfigValue($session, $name, $table = "ConfigUserAssoc")
    {
      $query = "select count(*) as c from $table where login='" . $session["login"] . "' " .
	" and name='$name'";
      $result = $this->configDB->query($query);
      $obj = $result->nextAssoc();
      $result->freeResult();
      return $obj["c"] > 0;
    }

  // FAB: doit etre deplacer dans la partie Session
  function getUsersList()
    {
      $query = "select * from User";
      return $this->get_config_idx_data("login", $query);
    }

  /**
   * \brief Changes the 
   */
  function changeConfigOwner($oldName, $newName, $table = "Config")
    {
      $query = "update " . $table . "UserAssoc set login='$newName' where login='$oldName'";
      $result = $this->configDB->query($query);
      $result->freeResult();
    }

  /**
   * \brief Returns the list of all the sections used for this configuration.
   */
  function getSectionsList()
    {
      $query = "select * from SectionsList";
      $result = $this->configDB->query($query);
      return $this->get_config_idx_data("name", $query);
    }

  /**
   * \brief Returns true if the specified user have write access
   *
   * \param session the session parameters
   * \param name (STRING) : the configuration parameter name
   * \param [table (STRING)] : the table name
   */
  function havePermission($session, $name, $table = "Config")
    {
      $query = "select count(*) as c from $table where name='$name' and access <= " . $session["access"];
      $result = $this->configDB->query($query);
      $obj = $result->nextAssoc();
      $result->freeResult();
      return $obj["c"] > 0;
    }

  /**
   * \private get_config_idx_data
   */
  function get_config_idx_data($idx, $query)
    {
      $result = $this->configDB->query($query);
      $data = array();
      while($obj = $result->nextAssoc())
	{
	  $data[$obj[$idx]] = $obj;
	}
      $result->freeResult();     
      return $data;
    }

  /**
   * \brief Close the connection
   */
  function close()
    {
      $this->configDB->closeDB();
    }
}
/** @} */
?>
