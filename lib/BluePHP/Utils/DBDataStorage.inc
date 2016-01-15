<?php

include_once("BluePHP/Utils/DBConnect.inc");
include_once("BluePHP/Utils/DataAccessor.inc");

/**
 * \class DBDataStorage
 * Base class for every class that manage some data stored into a SQL db.
 * \addtogroup Utils
 * @{
 */
class DBDataStorage implements DataAccessor
{
  /**
   * Connection value
   */
  var $con = false;
  /**
   * Connection parameter
   */
  var $conParams = false;
  /**
   * \todo what's this for ?
   */
  var $enquote = false;
  /**
   * module parameters
   */
  var $params = array( "name" => null,
		       "keyname" => null );

  /**
   * it's possible to share the DB con by passing
   * a ModuleProcessing by parameter with an existing connection
   * \param $params module parameters such as name, the keyname
   * \param $sharedCon share the DB connection
   */
  protected function __construct($params, $sharedCon = null)
  {
    $this->setParams($params);
    if($sharedCon)
      {
	$this->setCon($sharedCon->getCon());
      }
  }

  /**
   * Returns an array of all the data
   */
  public function getEntries()
  {
    return $this->getModuleEntries($this->params);
  }

  /**
   * Return a specific entry
   */
  public function getEntry($params)
  {
    return $this->getModuleEntry($params);
  }

  /**
   * insert new data
   * \param $params data values
   */ 
  function newEntry($params)
  {
    return $this->newModuleEntry($params);
  }

  /**
   * update existing data
   * \param $params new values
   */
  function setEntry($params)
  {
    return $this->setModuleEntry($params);
  }

  /**
   * remove data
   * \param $params data idx to be removed
   */
  function delEntry($params)
  {
    return $this->delModuleEntry($params);
  }

  /**
   * Retrieve the keyname
   */
  function getKey()
  {
    return $this->params["keyname"];
  }

  /**
   * \warning to check if this is used
   */
  function enquoteKey($v)
  {
    $this->enquote = $v;
  }

  /**
   * return a list of keys
   */
  function getKeys()
  {    
    return is_array($this->params["keyname"]) ? $this->params["keyname"] : explode(",", $this->params["keyname"]);
  }

  /**
   * return the module name
   */
  function getName()
  {
    return $this->params["name"];
  }

  /**
   * returns the connection
   */
  private function getCon()
  {
    return $this->con;
  }

  /**
   * set the connection
   * \param $con DB connection
   */
  private function setCon($con)
  {
    $this->con = $con;
  }

  /**
   * set the connection params
   * \param $params set the DB connection parameters
   */
  function setConParams($params)
  {
    $this->conParams = $params;
  }

  /**
   * Retrieve the connection parameters
   */
  function getConParams()
  {
    return $this->conParams;
  }

  /**
   * Create a select condition query based
   * on the parameters
   * \param $params data values
   * \param $sep sep string
   * \param $checkNum enquote only non numerical data (using is_numeric PHP)
   */
  function getKeyCond($params, $sep = " AND ", $checkNum = true)
  {
    $str = "";
    $keyList = $this->getKeys();
    $lastKey = end($keyList);
    foreach($keyList as $k => $v)
      {
	$curKey = $params[$v];
	if($checkNum)
	  {
	    $str .= $v . "=" . (is_numeric($curKey) ? $curKey : "\"$curKey\"");
	  }
	else
	  {
	    $str .= $v . "=" . $curKey;
	  }
	if($v != $lastKey)
	  {
	    $str .= $sep;
	  }
      }
    return $str;
  }

  /**
   * Check if data exists
   * \param $params data values
   */
  function haveKey($params)
  {
    $keyList = $this->getKeys();
    foreach($keyList as $k => $v)
      {
	if(!array_key_exists($v, $params))
	  {
	    return false;
	  }
      }
    return true;
  }

  /**
   * Returns the structure of the data managed
   */
  function getModuleStructure()
  {
    $this->connect();
    return $this->con->describes($this->getName());
  }

  /**
   * Returns module parameters
   */
  protected function getParams()
  {
    return $this->params;
  }

  /**
   * Change module parameters
   * \param $params the parameters
   */
  protected function setParams($params)
  {
    $this->params = $params;
  }

  /**
   * Select a set of data
   * \param $query query selection
   */
  protected function Select($query)
  {
    $res = array();
    $this->connect();
    $result = $this->con->query("select " . $query);
    while($obj = $result->nextAssoc())
      {
	$res[] = $obj;
      }
    $result->freeResult();
    return $res;    
  }  

  /**
   * Make a SQL statement
   * \param $query SQL statement
   */
  protected function QueryNoReturn($query)
  {
    $this->connect();
    $res = $this->con->query($query);
    $res->freeResult();
  }

  /**
   * SQL connection
   */
  protected function connect()
  {
    if(!$this->con)
      {
	if(!$this->conParams)
	  {
	    $this->conParams = array( "type" => CONFIGDBTYPE,
				      "host" => CONFIGDBHOST,
				      "name" => CONFIGDBNAME,
				      "user" => CONFIGDBUSER,
				      "passwd" => CONFIGDBPASSWD );
	  }
	$this->con = new DBConnect($this->conParams["type"],
				   $this->conParams["host"],
				   $this->conParams["name"],
				   $this->conParams["user"],
				   $this->conParams["passwd"]);
	$this->con->connectToDB();
      }
    return $this->con;
  }

  /**
   * Create the SQL statement that will return 
   * the entries
   * \param $params data values
   */
  private function getModuleEntries($params)
  {
    $res = array();
    $query = "select ";
    if(isset($params["fields"]) and $params["fields"])
      {
	$query .= $params["fields"];
      }
    else
      {
	$query .= "*";
      }

    $query .= " from " . $this->getName();
    if(isset($params["sort"]) and $params["sort"])
      {
	$query .= " order by " . $params["sort"];
      }   
    $this->connect();
    $result = $this->con->query($query);
    while($obj = $result->nextAssoc())
      {
	$res[] = $obj;
      }
    $result->freeResult();
    
    return $res;    
  }

  /**
   * Create the SQL statement that will return
   * a specific entry
   * \param $params data values
   */
  private function getModuleEntry($params)
  {
    $res = array();
    if($this->haveKey($params))
      {
	$query = "SELECT * FROM " . $this->getName() . 
	  " WHERE " . $this->getKeyCond($params);
	$this->connect();
	$result = $this->con->query($query);
	$res = $result->nextAssoc();
	$result->freeResult();
      }
    else
      {
	$res = array_merge($this->getModuleParams($params),
			   $params);
	$res = array_merge($res, $params);
      }
    return $res;
  }

  /**
   * Returns the data structure
   * For SQL use the table description
   * \param $params module params
   */
  protected function getModuleParams($params)
  {
    $res = array();
    $this->connect();
    $data = $this->con->describes($this->getName());
    foreach($data as $k => $v)
      {
	$res[$v["Field"]] = null; // empty parameter
      }
    return $res;
  }
    
  /**
   * Insert new data
   * \param $params data values
   */
  private function newModuleEntry($params)
  {
    $data = array_intersect_key($params,
				$this->getModuleParams( array( "name" => $this->getName() )));

    $this->connect();
    return $this->con->insert($this->getName(), $data);
  }

  /**
   * Update data
   * \param $params data values
   */
  private function setModuleEntry($params)
  {
    $data = array_intersect_key($params,
				$this->getModuleParams( array( "name" => $this->getName() )));
    $this->connect();
    return $this->con->update($this->getName(),
			      $this->getKeys(),
			      $data);
  }

  /**
   * Remove data
   * \param $params data values
   */
  private function delModuleEntry($params)
  {
    $this->connect();
    return $this->con->delete($this->getName(),
			      $this->getKeys(),
			      $params);
  }  
}

/** @} */

?>