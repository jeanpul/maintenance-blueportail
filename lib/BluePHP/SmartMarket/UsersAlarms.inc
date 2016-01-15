<?php

include_once("BluePHP/Utils/ModuleProcessing.inc");

class UsersAlarms extends ModuleProcessing
{

  function UsersAlarms()
  {
    parent::setParams( array( "name" => "UsersAlarms",
			      "keyname" => "user_id,alarm_id,site_id" ));
  }

  function getEntries()
  {    
    return parent::Select("Users.name as uname, Alarms.name as aname, " . 
			  "Sites.name as sname, sendEmail, sendSMS, " .
			  "user_id, alarm_id, site_id from Users, " . 
			  "Alarms, UsersAlarms, Sites " .
			  "where UsersAlarms.user_id=Users.id and " .
			  "UsersAlarms.alarm_id=Alarms.id and " . 
			  "UsersAlarms.site_id=Sites.id");
  }

  function getUsers($params)
  {
    return parent::Select("Users.name as uname, Alarms.name as aname, " .
			  "Sites.name as sname, sendEmail, sendSMS, " .
			  "user_id, alarm_id, site_id from Users," .
			  "Alarms, UsersAlarms, Sites " . 
			  "where user_id=Users.id and " . 
			  "alarm_id=Alarms.id and " . 
			  "site_id=Sites.id and " .
			  "alarm_id=" . $params["alarm_id"] . " and " .
			  "site_id=" . $params["site_id"]);
  }
}

?>
