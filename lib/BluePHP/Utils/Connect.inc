<?php

/* 
 * Connect.inc
 * Copyright: 2003 Blue Eye Video
 */

/**
 * Uses a persistent connection to the database, this avoid normally creating and deleting
 * a lot of ressource from MySQL. Returns the $=$connexion unique id that will be used
 * for future SQL processes.
 * Future version should includes postgres database support
 */
function connectToDB() 
{
  if(DBTYPE == 'SQLITE3')
  {
    ($connexion = sqlite3_open(DBNAME)) || die("Couldn't connect to SQLITE3 Database");
  }
  else
  {
    ($connexion = mysql_pconnect(DBHOST, DBUSER, DBPASSWD)) || die("Couldn't connect to MySQL");
    mysql_select_db(DBNAME, $connexion) || die("Couldn't connect to
    Database " . DBNAME);  
  }
  return $connexion;
  
} 

function Connexion($pServer, $pName, $pPasswd, $pBase)
{
  ($connexion = mysql_pconnect($pServer, $pName, $pPasswd))
    or die("Connexion au serveur ($pServer) impossible\n");
  
  if(!mysql_select_db($pBase, $connexion))
    {
      echo "Accès à la base ($pBase) impossible<br>";
      echo "<b>Message de MySQL : </b>" . mysql_error($connexion);
      exit;
    }  
  return $connexion;
}

function Query($pQuery, $pConnexion)
{
  if(DBTYPE == 'SQLITE3')
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
    ($result = $method($pConnexion, $pQuery)) ||
      die("Erreur de la requete ($pQuery)<br>".
	  "<b>Message de SQLITE3 :</b>".sqlite3_error($pConnexion));
  }
  else
  {
    ($result = mysql_query($pQuery, $pConnexion))
      or die("Erreur de la requete ($pQuery)<br>".
	     "<b>Message de MySQL :</b>".mysql_error($pConnexion));
  }
  return $result;
}

function AffectedRows($connexion)
{
  if(DBTYPE == 'SQLITE3')
  {
    die("NextObject : SQLITE3 : Not yet implemented");
  }
  else
  {
    return mysql_affected_rows($connexion);
  }
}

function NextObject($result)
{   
  if(DBTYPE == 'SQLITE3')
  {
    die("NextObject : SQLITE3 : Not yet implemented");
  }
  else
  {
    return mysql_fetch_object($result); 
  }
}

function NextRow($result)
{ 
  if(DBTYPE == 'SQLITE3')
  {
    return sqlite3_fetch($result);
  }
  else
  {
    return mysql_fetch_row($result);
  }
}

function NextAssoc($result)
{   
  if(DBTYPE == 'SQLITE3')
  {
    return sqlite3_fetch_array($result);
  }
  else
  {
    return mysql_fetch_assoc($result);
  }
}

function FreeResult($result)
{  
  if(DBTYPE == 'SQLITE3')
  {
    sqlite3_query_close($result);
  }
  else
  {
    mysql_free_result($result); 
  }
}

function closeDB($link)
{
  if(DBTYPE == 'SQLITE3')
  {
    sqlite3_close($link);
  }
  else
  {
    mysql_close($link);
  }
}

/**
 * delete all rows of a tabel
 * return the number of rows deleted
 */
function clearTable($pTableName, $pConnexion)
{
  $query="DELETE FROM " . $pTableName;
  if(Query($query, $pConnexion))
    {
      if(DBTYPE == 'SQLITE3')
	{
	  return -1;
	}
      else
	{
	  return AffectedRows($pConnexion);
	}
    }
  return 0;
}

?>