<?php 

/**
 * File: MySqlOps.inc
 * 
 * Common operations on mysql database used
 * by the php scripts.
 */


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
  ($result = mysql_query($pQuery, $pConnexion))
    or die("Erreur de la requete ($pQuery)<br>".
	   "<b>Message de MySQL :</b>".mysql_error($pConnexion));
  return $result;
}

function NextObject($result)
{ return mysql_fetch_object($result); }

function NextRow($result)
{ return mysql_fetch_row($result); }

function NextAssoc($result)
{ return mysql_fetch_assoc($result); }

function FreeResult($result)
{ mysql_free_result($result); }

function endKey($assoc)
{
  end($assoc);
  return key($assoc);
}

function updateAssoc($table, $assoc, $keyname, $connexion)
{
  $query = "UPDATE $table SET ";

  $updatepart = "";
  $wherepart = "WHERE $keyname=";

  $lastkey = endKey($assoc);
  foreach($assoc as $key => $value)
    {
      if($key == $keyname)
	{
	  $wherepart .= $value;
	}
      else if($key != "action")
	{
	  $updatepart .= $key . "=" . "\"" . $value . "\"";
	  if($key != $lastkey)
	    {
	      $updatepart .= ","; 
	    }
	}
    }
  
  $query .= $updatepart . " " . $wherepart;

  ($result = mysql_query($query, $connexion)) 
    or die("Erreur de la mise a jour $query<br>" .
	   "<b>Message de MySQL : </br>" . mysql_error($connexion));
  return $result;
}

function deleteWithKey($table, $keyname, $keyvalue, $connexion)
{
  $query = "DELETE FROM $table WHERE $keyname=" . $keyvalue;
  ($result = mysql_query($query, $connexion)) 
    or die("Erreur de la mise a jour $query<br>" .
	   "<b>Message de MySQL : </br>" . mysql_error($connexion));
  return $result;  
}

function insertAssoc($table, $assoc, $connexion)
{
  $query = "INSERT into $table";
  
  $colspart = "(";
  $valuespart = "VALUES (";

  $lastkey = endKey($assoc);
  foreach($assoc as $key => $value)
    {
      if($key != "action")
	{
	  $colspart .= $key;
	  $valuespart .= "\"" . $value . "\"";
	  if($key != $lastkey)
	    {
	      $colspart .= ",";
	      $valuespart .= ",";
	    }
	}
    }

  $colspart .= ")";
  $valuespart .= ")";

  $query .= " $colspart $valuespart";
  ($result = mysql_query($query, $connexion)) 
    or die("Erreur de l'insertion $query<br>" .
	   "<b>Message de MySQL : </br>" . mysql_error($connexion));
  return $result;
}

/**
 * Returns the binary data from
 * that will be inserted when a column
 * is of type longblob.
 * the key parameter is the column name
 * the binary contents must be uploaded with
 * the key name and must be into the FILES array.
 * value contains the key value when this is not a longblob
 */
//function getValueFromKey($table, $key, $value, $connexion)
//{
//  $query = "SHOW COLUMNS FROM $table LIKE '$key'";
//  
//  $result = mysql_query($query, $connexion) or die("Erreur de getValueFromKey" . 
//						   "<b>Message de MySQL : </br>" . mysql_error($connexion));
//  $col = NextObject($result);
//  if($col->Type == "longblob")
//    {
//      // the file value must be uploaded
//      $fid = fopen($_FILES[$key]['tmp_name'],"rb");
//      $data = addslashes(fread($fid, filesize($_FILES[$key]["tmp_name"])));
//      fclose($fd);
//      return $data;
//    }
//  return $value;
//}

function getEnumArrayFromColumn($table, $column, $connexion)
{
  $res = array();
  $query = "SHOW COLUMNS FROM $table LIKE '$column'";

  $result = mysql_query($query, $connexion) or die("Erreur de getEnumArrayFromColumn" . 
						   "<b>Message de MySQL : </br>" . mysql_error($connexion));
  $col = NextObject($result);
  if($col)
    {
      // parse column Type on the form enum('v1', 'v2', ...)
      $array = split("[.]*[']", $col->Type);
      for($i = 1; $i < count($array); $i += 2)
	{
	  $res[] = $array[$i];
	}
    }
  return $res;
}

function getFamillyIdFromProjectId($projectId, $connexion, $defaultFamillyId)
{
  $query = "select familly_id from familly_projects where project_id=$projectId";
  $result = Query($query, $connexion);
  $obj = NextObject($result);
  if($obj)
    {
      return $obj->familly_id;
    }
  return $defaultFamillyId;
}

function changeCIRValue($params)
{
  $action_id = $params["action_id"];
  $cir = $params["cir"];
  $query = "update action set cir='$cir' where action_id=$action_id";
  $result = Query($query, $params["connexion"]);
  return $result;
}

function changePlanValue($params)
{
  $action_id = $params["action_id"];
  $date = $params["date"];
  $dayPeriod = $params["dayPeriod"];
  $contact_id = $params["contact_id"];
  $query = "insert into planner_action values ('$date', $dayPeriod, $action_id, $contact_id) " .
    "on duplicate key update action_id=$action_id";
  $result = Query($query, $params["connexion"]);
}

?>
