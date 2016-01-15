<?php

define('DBCONNECT_SQLITE3_API', 0);
define('DBCONNECT_SQLITE3_CLASS', 1);
define('DBCONNECT_SQLITE3_PDO', 2);

class DBRes
{
  var $dbType;
  var $result;
  var $newsq3;

  function DBRes($pType, $pResult, $newsq3)
    {
      $this->dbType = $pType;
      $this->result = $pResult;
      $this->newsq3 = $newsq3;
    }
  
  function isValid()
    {
      return $this->result;
    }

  function nextObject()
    {   
      if($this->dbType == 'SQLITE3')
	{
	  die("NextObject : SQLITE3 : Not yet implemented");
	}
      else
	{
	  return mysql_fetch_object($this->result); 
	}
    }

  function nextRow()
    { 
      if($this->dbType == 'SQLITE3')
	{
	  if($this->newsq3 == DBCONNECT_SQLITE3_CLASS)
	    {
	      return $this->result->fetchArray(SQLITE3_NUM);
	    }
	  else if($this->newsq3 == DBCONNECT_SQLITE3_API)
	    {
	      return sqlite3_fetch($this->result);
	    }
	  else if($this->newsq3 == DBCONNECT_SQLITE3_PDO)
	    {
	      return $this->result->fetch(PDO::FETCH_OBJ);
	    }
	  else
	    {
	      die("DBRes:nextRow : Unmanaged sqlite3 version");
	    }
	}
      else
	{
	  return mysql_fetch_row($this->result);
	}
    }

  function nextAssoc()
    {   
      if($this->dbType == 'SQLITE3')
	{	  
	  if($this->newsq3 == DBCONNECT_SQLITE3_CLASS)
	    {
	      return $this->result->fetchArray(SQLITE3_ASSOC);
	    }
	  else if($this->newsq3 == DBCONNECT_SQLITE3_API)
	    {
	      return sqlite3_fetch_array($this->result);
	    }
	  else if($this->newsq3 == DBCONNECT_SQLITE3_PDO)
	    {
	      return $this->result->fetch(PDO::FETCH_ASSOC);
	    }
	  else
	    {
	      die("DBRes:nextAssoc : Unmanaged sqlite3 version");
	    }
	}
      else
	{
	  return mysql_fetch_assoc($this->result);
	}
    }

  function freeResult()
    {      
      if($this->dbType == 'SQLITE3')
	{
	  if(gettype($this->result) != 'boolean')
	    {
	      if($this->newsq3 == DBCONNECT_SQLITE3_CLASS)
		{
		  $this->result->finalize();
		}
	      else if($this->newsq3 == DBCONNECT_SQLITE3_API)
		{
		  sqlite3_query_close($this->result);
		} 
	      else if($this->newsq3 == DBCONNECT_SQLITE3_PDO)
		{
		  $this->result = null;
		}
	      else
		{
		  die("DBRes:freeResult : Unmanaged sqlite3 version");
		}
	    }
	}
      else if(gettype($this->result) != "boolean")
	{
	  mysql_free_result($this->result); 
	}
    }
}

/**
 * \class DBConnect
 * Manage a connection to a database
 */
class DBConnect 
{
  var $dbType;
  var $dbName;
  var $dbHost;
  var $dbUser;
  var $dbPasswd;
  
  var $connection = false;

  var $newsq3;

  var $errorStr;

  function DBConnect($pType, $pHost, $pName,
		     $pUser, $pPasswd)
    {
      $this->dbType= $pType;
      $this->dbHost = $pHost;
      $this->dbName = $pName;
      $this->dbUser = $pUser;
      $this->dbPasswd = $pPasswd;
      $this->newsq3 = false;
      $this->errorStr = null; 
    }

  function connectToDB()
    {  
      if($this->dbType == 'SQLITE3')
	{
	  if(function_exists('sqlite3_open'))
	    {
	      $this->newsq3 = DBCONNECT_SQLITE3_API;
	      $this->connection = sqlite3_open($this->dbName) ||
 		die("Couldn't connect to SQLITE3 Database");
	    } 
	  else 
	    {
	      try {
		$this->newsq3 = DBCONNECT_SQLITE3_PDO;
		$this->connection = new PDO('sqlite:' . $this->dbName);
	      } 
	      catch(PDOException $e) 
		{
		  $this->newsq3 = DBCONNECT_SQLITE3_CLASS;
		  $this->connection = new SQLite3($this->dbName, SQLITE3_OPEN_READWRITE);
		}
	      
	    }

	  if(!$this->connection)
	    {
	      die("DBConnect::connectToDB : Unmanaged sqlite3 version");
	    }
	}
      else
	{
	  ($this->connection = mysql_pconnect($this->dbHost, $this->dbUser, $this->dbPasswd)) || 
	    die("Couldn't connect to MySQL");
	  mysql_set_charset("utf8", $this->connection);
	  mysql_select_db($this->dbName, $this->connection) || 
	    die("Couldn't connect to Database " . $this->dbName);  
	}         
    }

  private function parse_integer_mysql($type)
  {
    $res = array( "Type" => "int", "Size" => 0 );
    preg_match('/(?:(int)\((\d+)\)|int)/', $type, $matches);
    if(count($matches) == 3)
      {
	$res["Size"] = $matches[2];
      }
    return $res;
  }

  private function parse_enum_mysql($type)
  {
    $res = array( "Type" => "enum", "Elts" => array() );
    preg_match('/(enum)\((.+)\)/', $type, $matches);
    if(count($matches) == 3)
      {
	$elts = explode(",", $matches[2]);
	foreach($elts as $k)
	  {
	    $res["Elts"][] = trim($k, "'");
	  }
      }
    return $res;
  }

  private function parse_varchar_mysql($type)
  {
    $res = array( "Type" => "varchar", "Size" => 0 );
    preg_match('/(?:(varchar)\((\d+)\)|varchar)/', $type, $matches);
    if(count($matches) == 3)
      {
	$res["Size"] = $matches[2];
      }
    return $res;
  }

  function describes($table)
  {
    $res = array();
    $result = null;
    if($this->dbType == 'SQLITE3')
      {
	$result = $this->query("pragma table_info('$table')");
	while($col = $result->nextAssoc())
	  {
	    $col["Field"] = $col["name"];	    
	    $col["Type"] = $col["type"];
	    $col["isInteger"] = $this->is_integer($col["Type"]);
	    $res[$col["Field"]] = $col;
	  }
	$result->freeResult();
      }
    else
      {
	$result = $this->query("show columns from $table");
	while($col = $result->nextAssoc())
	  {
	    $col["isInteger"] = $this->is_integer($col["Type"]);
	    if($col["isInteger"])
	      {
		$col = array_merge($col, $this->parse_integer_mysql($col["Type"]));
	      }
	    else if($this->is_enum($col["Type"]))
	      {
		$col = array_merge($col, $this->parse_enum_mysql($col["Type"]));
	      }
	    else if($this->is_varchar($col["Type"]))
	      {
		$col = array_merge($col, $this->parse_varchar_mysql($col["Type"]));
	      }
	    $res[$col["Field"]] = $col;
	  }
	$result->freeResult();
      }
    return $res;
  }

  function getErrorStr()
  {
    if($this->dbType == 'SQLITE3')
      {
	return sqlite3_error($this->connection);
      }
    else
      {
	return mysql_error($this->connection);
      }
  }

  function query($pQuery)
    {
      if($this->dbType == 'SQLITE3')
	{
	  $manips = 'INSERT|UPDATE|DELETE|REPLACE|'
	    . 'CREATE|DROP|'
	    . 'LOAD DATA|SELECT .* INTO|COPY|'
	    . 'ALTER|GRANT|REVOKE|'
	    . 'LOCK|UNLOCK';
	  if(preg_match('/^\s*"?(' . $manips . ')\s+/i', $pQuery))
	    {
	      $method = "sqlite3_exec";
	    }
	  else
	    {
	      $method = "sqlite3_query";
	    }
	  if($this->newsq3 == DBCONNECT_SQLITE3_CLASS || $this->newsq3 == DBCONNECT_SQLITE3_PDO)
	    {
	      if($method == "sqlite3_exec")
		{
		  $result = $this->connection->exec($pQuery);
		}
	      else
		{
		  $result = $this->connection->query($pQuery);
		}
	    }	  
	  else if($this->newsq3 == DBCONNECT_SQLITE3_API)
	    {
	      ($result = $method($this->connection, $pQuery)) ||
		die("Erreur de la requete ($pQuery)<br>".
		    "<b>Message de SQLITE3 :</b>".sqlite3_error($this->connection));
	    } 
	  else
	    {
	      die("DBConnect::query : Unmanaged sqlite3 version");
	    }
	}
      else
	{
	  $result = mysql_query($pQuery, $this->connection);
	  if(!$result)
	    {
	      return null;
	    }
	}
      return new DBRes($this->dbType, $result, $this->newsq3);
    }
  
  function affectedRows()
    {
      if($this->dbType == 'SQLITE3')
	{
	  if($this->newsq3 == DBCONNECT_SQLITE3_CLASS)
	    {
	      return $this->connection->changes();
	    }
	  else
	    {
	      die("NextObject : SQLITE3 : Not yet implemented for this version of sqlite3");
	    }
	}
      else
	{
	  return mysql_affected_rows($this->connection);
	}
    }

  function closeDB()
    {
      if($this->dbType == 'SQLITE3')
	{
	  if($this->newsq3 == DBCONNECT_SQLITE3_CLASS)
	    {
	      $this->connection->close();
	    }
	  else if($this->newsq3 ==  DBCONNECT_SQLITE3_PDO)
	    {
	      $this->connection = null;
	    }
	  else if($this->newsq3 ==  DBCONNECT_SQLITE3_API)
	    {
	      sqlite3_close($this->connection);
	    }
	  else
	    {
	      die("DBConnect::closeDB : Umanaged sqlite3 version");
	    }
	}
      else
	{
	  mysql_close($this->connection);
	}
    }

  /**
   * delete all rows of a table
   * return the number of rows deleted
   */
  function clearTable($pTableName)
    {
      $query="DELETE FROM " . $pTableName;
      if($this->query($query))
	{
	  if($this->dbType == 'SQLITE3')
	    {
	      return -1;
	    }
	  else
	    {
	      return $this->affectedRows();
	    }
	}
      return 0;
    }

  function isTableExist($pTableName)
  {
    $query = false;
    if($this->dbType == 'SQLITE3')
      {
	$query = "select name from sqlite_master where name='$pTableName'";
      }
    else
      {
	  $query = "show tables like '$pTableName'";
      }
    
    if($query)
      {
	$result = $this->query($query);
	$obj = $result->nextAssoc();
	$result->freeResult();
	return $obj;
      }
    return false;
  }

  function is_integer($type)
  {
    if($this->dbType == 'SQLITE3')
      {
	return $type == "INTEGER";
      }
    else
      {
	$p = strpos($type, "int");
	return  is_integer($p) && $p == 0;
      }
  }

  function is_enum($type)
  {
    if($this->dbType == 'SQLITE3')
      {
	die("is_enum for SQLITE3 not implemented in DBConnect.inc");
      }
    else
      {
	$p = strpos($type, "enum");
	return is_integer($p) && $p == 0;
      }
  }

  function is_varchar($type)
  {
    if($this->dbType == 'SQLITE3')
      {
	die("is_varchar for SQLITE3 not implemented in DBConnect.inc");
      }
    else
      {
	$p = strpos($type, "varchar");
	return is_integer($p) && $p == 0;
      }
  }

  function insert($table, $data)
  {
    $desc = $this->describes($table);
    $cols = "(";
    $values = "values (";
    $datakeys = array_keys($data);
    $lastkey = end($datakeys);
    foreach($data as $k => $v)
      {
	$cols .= $k;
	$values .= $desc["$k"]["isInteger"] ? $v : "\"$v\"";
	if($lastkey != $k)
	  {
	    $cols .= ",";
	    $values .= ",";
	  }
      }
    $cols .= ")";
    $values .= ")";
    $query = "insert into $table $cols $values";
    return $this->query($query);
  }

  public function generateEltsQuery($table, $data, $sep = " and ")
  {
    $desc = $this->describes($table);
    $firstElt = true;
    $str = "";
    foreach($data as $k => $v)
      {
	if(!$firstElt)
	  {
	    $str .= $sep;
	  }
	else
	  {
	    $firstElt = false;
	  }
	$str .= "$k=" . ($desc["$k"]["isInteger"] ? $v : "\"$v\"");
      }
    return $str;
  }

  function update($table, $keys, $data)
  {
    $desc = $this->describes($table);
    $up = "";
    $wp = "";
    $firstKey = true;
    $firstData = true;
    foreach($data as $k => $v)
      {
	if(in_array($k, $keys))
	  {
	    if(!$firstKey)
	      {
		$wp .= " and ";
	      }
	    else
	      {
		$firstKey = false;
	      }
	    $wp .= "$k=" . ($desc["$k"]["isInteger"] ? $v : "\"$v\"");
	  }
	else
	  {
	    if(!$firstData)
	      {
		$up .= ", ";
	      }
	    else
	      {
		$firstData = false;
	      }
	    $up .= "$k=" . ($desc["$k"]["isInteger"] ? $v : "\"$v\"");
	  }
	}
    $query = "update $table set $up where $wp";
    return $this->query($query);
  }

  function delete($table, $keys, $data)
  {
    $desc = $this->describes($table);
    $cond = "";
    $lastKey = end($keys);
    foreach($keys as $k => $v)
      {
	$cond .= "$v=" . ( $desc["$v"]["isInteger"] ? $data[$v] : "\"" . $data[$v] . "\"");
	if($v != $lastKey)
	  {
	    $cond .= " and ";
	  }
      }
    $query = "delete from $table where $cond";
    return $this->query($query);
  }

  function getAssociativeArray($result, $idx = false)
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
}

?>
