<?php

include_once("DBConnect.inc");

/**
 * Check if the user login and the access right are OK
 * Access levels are specified in the $PageAccess structure into the Forms.inc file.
 */
function userSearch($login, $accessLevel, $connection)
{
  $query = "SELECT * FROM User WHERE login = '$login' AND access >= $accessLevel";
  $result = $connection->query($query);
  return $result->nextAssoc();
}

/**
 * Check if the session id and the access right are OK
 * Access levels are specified in the $PageAccess structure into the Forms.inc file.
 */
function sessionSearch($sessionId, $accessLevel, $connection)
{
  $query = "SELECT * FROM Session WHERE sessionid = '$sessionId' AND access >= $accessLevel";
  $result = $connection->query($query);
  return $result->nextAssoc();
}

/**
 * Check if the session is time valid and the access right are OK
 * Access levels are specified in the $PageAccess structure into the Forms.inc file.
 * The session expired at a certain time limit defined into the sessionCreate function below.
 */
function sessionValid($session, $accessLevel, $connection)
{
  global $errorMsg;

  // check access level
  if($session["access"] < $accessLevel)
    {
      return FALSE;
    }

  // check time session
  $now = date("U");
  if($session["timelimit"] != 0 and $session["timelimit"] < $now)
    {
      /** PHP session destroy */
      session_destroy();
      $query = "DELETE FROM Session WHERE sessionId='" . $session["sessionid"] . "'";
      $result = $connection->query($query);
      $result->freeResult();
      return FALSE;
    }

  // delete all old session
  $query = "DELETE FROM Session WHERE timelimit < $now";
  $result = $connection->query($query);
  $result->freeResult();
  return TRUE;
}

/**
 * Destroy a session when the user log out the application.
 */
function sessionDestroy($sessionId, $connection)
{
  $currentSession = sessionSearch($sessionId, 0, $connection);

  if(is_array($currentSession))
    {
      /** PHP session destroy */
      session_destroy();

      $query = "DELETE FROM Session WHERE sessionId='$sessionId'";
      $result = $connection->query($query);
      $result->freeResult();
    }
  
  return TRUE;
}

/**
 * When the user has log in and has access right to the application,
 * we create a user session composed of a PHP session that provides cookie functionality
 * and a unique id, and a MySQL session entry to provides time limited session.
 * By default a 1 hour session is given.
 * User password is not encrypted into the database
 * WARNING ! using http and not https means that the login/password are sended
 * in a non encrypted form from the client to the http server !
 */
function sessionCreate($connection, $login, $passwd, $sessionId, $accessLevel, $sessionTime)
{
  global $errorMsg;

  // check the user existence into the database
  $user = userSearch($login, $accessLevel, $connection);
  if(is_array($user))
    {
      // check the password 
      if($user["passwd"] == $passwd)
	{
	  // Create a new session
	  $now = date("U");
	  $timelimit = 0;

	  if($sessionTime > 0)
	    {
	      $timelimit = $now + $sessionTime; 
	    }
	  
	  $query = "INSERT INTO Session (sessionid, login, timelimit, access) "
	    . "VALUES ('$sessionId', '$login', '$timelimit', '" . $user["access"] . "')";
	  $result = $connection->query($query);
	  $result->freeResult();

	  return TRUE;
	}
      return FALSE;
    }
  else
    {
      return FALSE;
    }
}

/**
 * This function is called by main scripts to check user access before displaying anything.
 * Remember to call this function first before processing anything !
 */
function checkAccess($scriptName, $loginInfo, $sessionId, $wantHeader = FALSE, $accessLevel = 0, $sessionTime = false)
{
  global $errorMsg;
  
  // Database connection
  $connection = new DBConnect(SESSION_DBTYPE,
			      SESSION_DBHOST,
			      SESSION_DBNAME,
			      SESSION_DBUSER,
			      SESSION_DBPASSWD);
  $connection->connectToDB();

  // If a session already exist, directly return the session id otherwise
  // we check the login/password 
  $currentSession = sessionSearch($sessionId, 0, $connection);
  if(is_array($currentSession))
    {
      // avoid multiple connection on the same session id
      if(isSet($loginInfo["login"]))
	{
	  $errorMsg[] = "ErrorMultiple";
	}
      else if(sessionValid($currentSession, $accessLevel, $connection))
	{
	  return $currentSession;
	}
      else
	{
	  $errorMsg[] = _("ErrorSession");
	}
    }
  else if(isSet($loginInfo["login"]))
    {
      if(!is_int($sessionTime))
	{
	  $sessionTime = SESSIONTIME;
	}

      // No session exit but a login/passwd data pair has been
      // entered and POSTED by HTTP so check it
      if(sessionCreate($connection, $loginInfo["login"],
		       $loginInfo["passwd"], $sessionId, $accessLevel, $sessionTime))
	{
	  return sessionSearch($sessionId, $accessLevel, $connection);
	}
      else
	{
	  $errorMsg[] = _("ErrorLogin");
	}
    }

  // No session, no login/passwd so display the login box
  //  formAuth($scriptName, $wantHeader);

  // Close connection, we open a new connexion
  // when the user enter login/passwd to prevent a opened connection
  // to be present outside the user session.
  $connection->closeDB();

  return FALSE;
}

/**
 * This function destroy the session defined in the database
 */
function removeCurrentAccess()
{
  // Database connection
  $connection = new DBConnect(SESSION_DBTYPE,
			      SESSION_DBHOST,
			      SESSION_DBNAME,
			      SESSION_DBUSER,
			      SESSION_DBPASSWD);
  $connection->connectToDB();
  sessionDestroy(session_id(), $connection);
  $connection->closeDB();
}

/**
 * This function returns the associative array
 * containing the User table entries
 */
function getUsersIds()
{
  $connection = new DBConnect(SESSION_DBTYPE,
			      SESSION_DBHOST,
			      SESSION_DBNAME,
			      SESSION_DBUSER,
			      SESSION_DBPASSWD);
  $connection->connectToDB();

  $query = "select login, passwd, access from User";
  $result = $connection->query($query);
  $data = array();
  while($obj = $result->nextAssoc())
    {
      $data[] = $obj;
    }
  $result->freeResult();
  $connection->closeDB();
  return $data;
}

/**
 * This function update the parameters of a specific
 * user. The function check if the login is unique
 * and returns false in case of error. The function
 * also update the Session table.
 */
function updateUserValues($values)
{
  $connection = new DBConnect(SESSION_DBTYPE,
			      SESSION_DBHOST,
			      SESSION_DBNAME,
			      SESSION_DBUSER,
			      SESSION_DBPASSWD);
  $connection->connectToDB();
  //  return "<h2>" . $values["login"] . " " . $values["prevLogin"] . "</h2>";
  // if the login change check if it is unique
  if($values["login"] != $values["prevLogin"])
    {
      $query = "select login from User where login='" . $values["login"] . "'";
      $result = $connection->query($query);
      if($obj = $result->nextAssoc())
	{ 
	  $result->freeResult();
	  $connection->closeDB();
	  return false;
	}
      $result->freeResult();
    }
  // update all the user parameters
  $accessStr = isset($values["access"]) ? ", access='" . $values["access"] . "'" : "";

  $query = "update User set login='" . $values["login"] . "'" .
    ", passwd='" . $values["passwd"] . "'" . $accessStr .
		     " where login='" . $values["prevLogin"] . "'";

  $result = $connection->query($query);

  if(!$result)
    {
      $result->freeResult();
      $connection->closeDB();
      return false;
    }
  $result->freeResult();
  // update the session table if necessary
  if($values["login"] != $values["prevLogin"])
    {
      $query = "update Session set login='" . $values["login"] . "'" . 
	" where login='" . $values["prevLogin"] . "'";
      $result = $connection->query($query);
      if(!$result)
	{
	  $result->freeResult();
	  $connection->closeDB();
	  return false;
	}
      $result->freeResult();
    }
  $connection->closeDB();
  return true;
}

?>
