<?php

include_once("BluePHP/Utils/DBConnect.inc");

/**
 * \file BluePortailLang.inc
 * \brief Interface to BluePortail data
 */

/**
 * \class BluePortailLang
 * \brief Interface to BluePortail data
 *
 * Use the configuration file to manage BluePortail
 * data
 */
/** \addtogroup BluePortailLang */
/** @{ */
class BluePortailLang
{
  /**
   * \private configDB
   * Connection to the config server
   */
  var $configDB;

  /**
   * \brief Create the BluePortailLang interface
   */
  function BluePortailLang($paramsDB = FALSE)
    {
      if($paramsDB === FALSE)
	{

	  $this->configDB = new DBConnect(CONFIGDBTYPE, CONFIGDBHOST, CONFIGDBNAME,
					  CONFIGDBUSER, CONFIGDBPASSWD);
	}
      else
	{
	  $paramsDB = array_merge(array("TYPE" => "SQLITE3",
					"HOST"=> "localhost",
					"NAME" => "/home/DATA/BluePortail/session.db",
					"USER" => "blue",
					"PASSWD" => "blue4eye"),
				  $paramsDB);
	  $this->configDB = new DBConnect($paramsDB["TYPE"], $paramsDB["HOST"], $paramsDB["NAME"], $paramsDB["USER"], $paramsDB["PASSWD"]);
	}
      $this->configDB->connectToDB();
    }

  public function setClientId($clientId)
  {
    $_REQUEST = array();
    $_REQUEST["clientBluePortail"] = $clientId;  
  }

  /**
   * \brief call any method of the BluePortailLang class through its symbolic name
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
   * \brief Returns the URL of the remote BluePortailLang to access a specified function
   */
  function callRemoteFunc($server, $funcname, $params)
    {
      $url = $server . "/BluePortailAdmin/BluePortailLang.php?function=$funcname";
      foreach($params as $k => $v)
	{
	  $url .= "&$k=".urlencode($v);
	}
      return $url;
    }
  

  /**
   * \brief returns the language version
   */
  function version($params = "")
    {
      return array ( "version" => "beta" );
    }

  /**
   * \brief Returns the BlueCount server status
   */
  function getServerStatus($params)
    {
      $handle = @fopen($params["server"], "r");
      if($handle)
	{
	  fclose($handle);
	  return true;
	}
      return false;
    }

  function getBlueHTTPServerStatus($params)
    {
      // create the BlueHTTP message from the ref value
      list($client, $type, $channel, $object) = split("_", $params["bluecountId"]);
      $msg = $params["server"] . "/BTopLocalServer/GrabCounter.php?client=$client&channel=$channel";

      if($type == "B-TOP")
	{
	  $msg .= "&type=144&counter=$object&timeStart=20070101010000&timeEnd=20070101020000";
	}
      else if($type == "B-QUEUE")
	{
	  $msg .= "&type=145&region=$object&timestamp=20070101010000";
	}
      else
	{
	  return false;
	}
      $msg .= "&value=0&test=1";
      $handle = @fopen($msg, "r");
      if($handle)
	{
	  fclose($handle);
	  return true;
	}
      return false;
    }

  /**
   * \brief Returns the parameters of a specified Client.
   *
   * Returns the parameters of a specified Client.
   * \param $params the parameters array which contains : 
   * - clientId (STRING) : the unique Client id
   * \return Then the function returns an array that contains : 
   * - clientId (STRING) : the unique Client login
   * - passwd (STRING) : the Client password
   * - email (STRING) : the Client email
   * - server (STRING) : the URL of the BlueCount server
   * - access (INT >= 0) : the Client access level
   */
  function getClientData($params)
    {
      $query = "select Client.id as clientId, Client.name as clientName, email, value as server, access " . 
	"from Client, ConfigClientAssoc where Client.id=ConfigClientAssoc.clientId and " .
	"ConfigClientAssoc.name=\"bluecountserver\" and Client.id=\"" . $params["clientId"] . "\"";
      return $this->get_config_array($query);
    }

  /**
   * \brief Returns the list of available clients.
   *
   * Returns the list of the available clients.
   * \return The array containing the clients where each elements contains : 
   * - clientId (STRING) : the unique Client login
   * - passwd (STRING) : the Client password
   * - email (STRING) : the Client email
   * - server (URL) : the Client BlueCountServer location
   */
  function getClients($params = "")
    {
      $query = "select Client.id as clientId, Client.name as clientName, email, value as server " . 
	"from Client, ConfigClientAssoc where Client.id=ConfigClientAssoc.clientId and ConfigClientAssoc.name=\"bluecountserver\"";
      
      return $this->get_config_array($query);
    }

  /**
   * \brief Create a new client.
   *
   * Create a new client.
   * \params $params the parameters array which contains :    
   * - clientName (STRING) : the client name
   * - email (STRING) : the client email
   * - access (INT >= 0) : the client access level
   * - server (URL) : the Client BlueCountServer location
   * \return the new client parameters data in an array list @see getClientData
   */
  function processClientInsert($params)
    {
      if(!isset($params["clientId"]) or $params["clientId"] == "")
	{
	  $output = array();
	  $strdata = $params["clientName"] . " " . strftime("%Y%m%d%H%M%S");
	  $tmpfile = "/tmp/processClientInsert";
	  $fileHandle = fopen($tmpfile, "w");
          if($fileHandle)
          {
	    fwrite($fileHandle, $strdata);
	    fclose($fileHandle);
          }
	  $cmd = "md5sum " . $tmpfile;
	  exec($cmd, $output);
	  unlink($tmpfile);	  
	  $params["clientId"] = substr($output[0], 0, 16);	  
	}

      $query = "insert into Client(id, name, email, access) values(\"" . $params["clientId"] . "\"," .
	"\"" . $params["clientName"] . "\"," .
	"\"" . $params["email"] . "\"," . 
	$params["access"] . ")";
      $result = $this->configDB->query($query);
      $result->freeResult();

      $query = "insert into ConfigClientAssoc(name, clientId, value) values(\"bluecountserver\"" . 
	",\"" . $params["clientId"] . "\",\"" . $params["server"] . "\")";
      $result = $this->configDB->query($query);
      $result->freeResult();

      // add the link between the client and the host for the access to the data via the proxy
      exec(SCRIPTSPATH . "/clientManageClientDest.sh " . CLIENTHOSTFILE . " add " . $params["clientId"] . " " . $params["server"]);

      // create the client data using the remote BluePortailLang
      if(!file_get_contents($this->callRemoteFunc($params["server"], "processClientCreateData", $params)))
	{
	  syslog(LOG_ERROR, "BluePortailAdmin:processClientInsert cannot call processClientCreateData");
	}

      return array( $params );
    }  

  /**
   * \brief Delete an existing Client.
   */
  function processClientDelete($params)
    {
      $query = "delete from Client where id=\"" . $params["clientId"] . "\"";
      $result = $this->configDB->query($query);
      $result->freeResult();

      exec(SCRIPTSPATH . "/clientManageClientDest.sh " . CLIENTHOSTFILE . " del " . $params["clientId"]);

      exec(SCRIPTSPATH . "/clientManageKey.sh delClient 0 " . $params["clientId"]);

      // remove the client data using the remote BluePortailLang
      if(!file_get_contents($this->callRemoteFunc($params["server"], "processClientDeleteData", $params)))
	{
	  syslog(LOG_ERROR, "BluePortailAdmin:processClientInsert cannot call processClientDeleteData");
	}    

      return array( array( $params["clientId"] ) );
    }

  /**
   * \brief Changes the parameters values of an existing Client
   * Warning : If you change the server value then this will erase
   * the data of the client on the old server and create a new Client. 
   * THE DATA WILL BE COMPLETLY LOST
   */
  function processClientUpdate($params)
    {
      // if the server change then use processClientDelete
      // and processClientInsert otherwise only update values
      // in the database

      // retrieve the current server name
      $oldValues = $this->getClientData($params);
      if($oldValues[0]["server"] == $params["server"])
	{
	  $query = "update Client set name=\"" . $params["clientName"] . "\"" . 
	    ",email=\"" . $params["email"] . "\"" .
	    " where id=\"" . $params["clientId"] . "\"";
	  $result = $this->configDB->query($query);
	  $result->freeResult();

	  $query = "update ConfigClientAssoc set value=\"" . $params["server"] . "\"" .
	    " where name=\"bluecountserver\" and clientId=\"" . $params["clientId"] . "\"";
	  $result = $this->configDB->query($query);
	  $result->freeResult();
	}
      else
	{
	  $newServer = $params["server"];
	  $params["server"] = $oldValues[0]["server"];
	  $this->processClientDelete($params);
	  $params["server"] = $newServer;
	  $this->processClientInsert($params);
	}
      return array( $params );
    }

  /**
   * \brief Create the client data.
   * \params $params the parameters array which contains : 
   * clientId (STRING) : the client blueportail unique id
   */
  function processClientCreateData($params)
    {
      exec(SCRIPTSPATH . "/clientCreate.sh " . $params["clientId"]);
      return array( $params );
    }

  /**
   * \brief Remove the client data.
   * \params $params the parameters array which contains : 
   * clientId (STRING) : the client blueportail unique id
   */
  function processClientDeleteData($params)
    {
      exec(SCRIPTSPATH . "/clientDelete.sh " . $params["clientId"]);
      return array( $params );
    }

  /**
   * \brief Returns the parameters of a specified Key.
   *
   * Returns the parameters of a specified Key.
   * \param $params the parameters array which contains : 
   * - keyId (STRING) : the unique key id
   * \return Then the function returns an array that contains : 
   * - keyId (STRING) : the unique key id
   * - clientId (STRING) : the unique Client login
   * - propName (STRING) : the key proprietary name
   * - propEmail (STRING) : the key proprietary email
   */
  function getKeyData($params)
    {
      $query = "select id as keyId, startDate, endDate, clientId, name as propName, email as propEmail " . 
	"from SSLKey where id=\"" . $params["keyId"] . "\"";
      return $this->get_config_array($query);
    }

  /**
   * \brief Returns the list of available keys.
   *
   * Returns the list of the available keys.
   * \return The array containing the keys where each elements contains : 
   * - keyId (STRING) : the unique Key id
   * - clientId (STRING) : the unique Client login
   * - propName (STRING) : the proprietary name
   * - propEmail (STRING) : the proprietary email
   */
  function getKeys($params = "")
    {
      $query = "select id as keyId, startDate, endDate, clientId, name as propName, email as propEmail from SSLKey";
      return $this->get_config_array($query);
    }

  /**
   * \brief Create a new key.
   *
   * Create a new key.
   * \params $params the parameters array which contains :    
   * - keyId (STRING) : the unique key Id
   * - clientId (STRING) : the unique client Id
   * - propName (STRING) : the proprietary name
   * - propEmail (STRING) : the proprietary email
   * \return the new key parameters data in an array list @see getKeyData
   */
  function processKeyInsert($params)
  {
    $query = "insert into SSLKey(id, clientId, startDate, endDate, name, email) values(\"" . $params["keyId"] . "\"," . 
      "\"" . $params["clientId"] . "\"," .
      "\"" . $params["startDate"] . "\"," .
      "\"" . $params["endDate"] . "\"," .
      "\"" . $params["propName"] . "\"," . 
      "\"" . $params["propEmail"] . "\")";
    $result = $this->configDB->query($query);
    $result->freeResult();

    // convert date from YYYY-MM-DD [HH:MM:SS] to YYMMDDHHMMSSU 
    // @see openssl ca manual page for startdate and enddate parameters
    $startDate = strftime("%y%m%d%H%M%S", strtotime($params["startDate"])) . "Z";
    $endDate = strftime("%y%m%d%H%M%S", strtotime($params["endDate"])) . "Z";

    // create the key file with the CA signature
    exec(SCRIPTSPATH . "/clientManageKey.sh add " . $params["keyId"] . " " . $params["clientId"] . " " . $startDate . " " . $endDate);

    return array( $params );
  }

  /**
   * \brief Delete an existing Key.
   */
  function processKeyDelete($params)
    {
      $query = "delete from SSLKey where id=\"" . $params["keyId"] . "\"";
      $result = $this->configDB->query($query);
      $result->freeResult();

      exec(SCRIPTSPATH . "/clientManageKey.sh del " . $params["keyId"] . " " . $params["clientId"]);

      return array( array( $params["keyId"] ) );
    }

  /**
   * \brief Changes the parameters values of an existing Key
   * WARNING : The clientId could not be changed
   */
  function processKeyUpdate($params)
    {
      $query = "update SSLKey set name=\"" . $params["propName"] . "\"" . 
	",email=\"" . $params["propEmail"] . "\"";
      $result = $this->configDB->query($query);
      $result->freeResult();
      return array( $params );
    }

  /**
   * \brief Returns the list of parameters of a specified BlueCount
   *
   * Returns the list of parameters of a specified BlueCount on element for each
   * client component.
   * \param $params the parameters array which contains :
   * - ref (STRING) : the unique BlueCount reference
   * \return Then the function returns list of elements which contains :
   * - ref (STRING) : the unique BlueCount reference
   * - clientId (INT >= 0) : the unique Client id associated with
   * - isActivated (0 | 1) : whether or not this BlueCount is activated
   */
  function getBlueCountData($params)
    {
      if(isset($params["clientId"]))
	{
	  $query = "select BlueCountClientAssoc.clientId AS clientId, BlueCountAvailable.ref AS ref, " . 
	    "BlueCountClientAssoc.isActivated AS isActivated FROM BlueCountAvailable, BlueCountClientAssoc" .
	    " where BlueCountClientAssoc.clientId=\"" . $params["clientId"] . "\" and BlueCountAvailable.ref=BlueCountClientAssoc.bluecountId";
	}
      else if(isset($params["ref"]))
	{
	  $query = "select BlueCountClientAssoc.clientId AS clientId, BlueCountAvailable.ref AS ref, " . 
	    "BlueCountClientAssoc.isActivated AS isActivated FROM BlueCountAvailable, BlueCountClientAssoc" .
	    " where BlueCountAvailable.ref=\"" . $params["ref"] . "\" and BlueCountAvailable.ref=BlueCountClientAssoc.bluecountId";
	}
      return $this->get_config_array($query);
    }

  /**
   * \brief Returns the list of BlueCount sensors
   * 
   * Returns the list of available BlueCount
   * \return the list of available BlueCount where each element contains : 
   * - ref (STRING) : the unique BlueCount reference
   */
  function getBlueCounts($params = "")
    {
      $query = "select ref from BlueCountAvailable";
      return $this->get_config_array($query);
    }

  /**
   * \brief Insert a new BlueCount
   *
   * Insert a new BlueCount with its connected client. By default
   * all the BlueCount/client connection are activated.
   * \param $params the parameters array which contains : 
   * - ref (STRING) : the unique BlueCount reference
   * - clients (ARRAY OF STRING) : an array of all the clients linked to this BlueCOunt
   * \return the new BlueCount parameters data in an array list @see getBlueCountData
   */
  function processBlueCountInsert($params)
    {
      $query = "insert into BlueCountAvailable values(\"" . $params["ref"] . "\")";
      $result = $this->configDB->query($query);
      $result->freeResult();

      foreach($params["clients"] as $v)
	{
	  $query = "insert into BlueCountClientAssoc(bluecountId, clientId, isActivated) values(\"" . $params["ref"] . "\"" .
	",\"" . $v . "\",1)";
	  $result = $this->configDB->query($query);
	  $result->freeResult();
	}

      return array( $params );
    }

  /**
   * \brief Delete a BlueCount
   *
   * Delete a BlueCount with its Client connection.
   * \param $params the parameters array which contains : 
   * - ref (STRING) : an unique sensor reference
   * \returns the reference of the delete sensor
   */
  function processBlueCountDelete($params)
    {
      $query = "delete from BlueCountAvailable where ref=\"" . $params["ref"] . "\"";
      $result = $this->configDB->query($query);
      $result->freeResult();
      
      return array( $params );
    }

  /**
   * \brief Changes the parameters values of the specified BlueCount.
   *
   * Changes the parameters values of the specified BlueCount which is in the bluecount
   * client association list.
   * \param $params the parameters array which contains : 
   * - ref (STRING) : the unique BlueCount reference
   * - clients (ARRAY OF STRING) : an array of all the clients linked to this BlueCOunt
   * \returns the new sensor parameters data @see getBlueCountData
   */
  function processBlueCountUpdate($params)
    {
      // delete previous client components
      $query = "delete from BlueCountClientAssoc where bluecountId=\"" . $params["ref"] . "\"";
      $result = $this->configDB->query($query);
      $result->freeResult();

      // insert new client components
      foreach($params["clients"] as $v)
	{
	  $query = "insert into BlueCountClientAssoc(bluecountId, clientId, isActivated) values(\"" . $params["ref"] . "\"" .
	",\"" . $v . "\",1)";
	  $result = $this->configDB->query($query);
	  $result->freeResult();
	}

      return array( $params );
    }

  /**
   * \brief Returns the list of BlueCount Client connections
   *
   * Returns the list of BlueCount Client connections
   * \return A list where each components contains : 
   * - blueCountId (STRING) : the unique BlueCount reference
   * - clientId (STRING) : the unique Client id associated with
   * - server (STRING) : the URL of the BlueCount server
   * - isActivated (0 | 1) : whether or not this connection is activated
   */
  function getLinks($params = "")
    {
      $query = "select BlueCountClientAssoc.bluecountId, " . 
	"BlueCountClientAssoc.clientId, isActivated, value as server " . 
	"from BlueCountClientAssoc, ConfigClientAssoc " .
	"where BlueCountClientAssoc.clientId=ConfigClientAssoc.clientId " . 
	"and ConfigClientAssoc.name=\"bluecountserver\"";
      return $this->get_config_array($query);
    }

  /**
   * \brief Change the activation parameter of a BlueCount/Client connection   
   * \param $params the parameters array which contains : 
   * - blueCountId (STRING) : the unique BlueCount reference
   * - clientId (STRING) : the unique Client id associated with
   * - isActivated (0 | 1) : whether or not this connection is activated
   */
  function processUpdateLink($params)
    {
      $query = "update BlueCountClientAssoc set isActivated=" . $params["isActivated"] . 
	" where bluecountId=\"" . $params["bluecountId"] . "\" and clientId=\"" . $params["clientId"] . "\"";
	  $result = $this->configDB->query($query);
	  $result->freeResult();

      return array( $params );
    }

  /**
   * \brief Update information about the IP adress
   * of the client that send data   
   */
  function updateIPTable($params)
  {
    $query = "replace into GrabIPTable values(\"" . $params["bluecountId"] 
      . "\",\"" . $params["clientId"] . "\",\"" . $params["lasttimestamp"] 
      . "\",\"" . $params["laststatus"] . "\",\"" . $params["lastIP"] .  "\")";
    $result = $this->configDB->query($query);
    $result->freeResult();
    return array($params);
  }

  /**
   * \brief Returns counter status information
   */
  function getIPTable($params = array())
  {
    $query = "select * from GrabIPName, GrabIPTable where GrabIPName.bluecountId=GrabIPTable.bluecountId" . (isset($params["extra"]) ? " " . $params["extra"] : "");
    $result = $this->configDB->query($query);
    $data = $this->get_associative_array($result, false);
    $result->freeResult();
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
      $result = $this->configDB->query($query);
      $data = $this->get_associative_array($result, $idx);
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
   * \brief Close the connection
   */
  function close()
    {
      $this->configDB->closeDB();
    }
}
/** @} */
?>
