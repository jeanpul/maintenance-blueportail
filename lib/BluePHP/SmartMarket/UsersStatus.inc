<?php

include_once("BluePHP/Utils/ModuleProcessing.inc");
require_once("Alarms.inc");

class UsersStatus extends ModuleProcessing
{

  function UsersStatus()
  {
    parent::setParams( array( "name" => "UsersStatus",
			      "keyname" => "user_id,alarm_id,site_id,site_entity_id",
			      "sort" => "user_id" ));
  }

  function getEntries()
  {
    return parent::Select("user_id, alarm_id, site_id, site_entity_id, " .
			  "last_mail_raised_date, last_sms_raised_date, " .
			  "nb_mail_raised, nb_sms_raised, last_sms_msg, last_mail_msg, " .
			  "Users.name as uname, Alarms.name as aname, Sites.name as sname " .
			  "from Alarms, Users, Sites, UsersStatus " . 
			  "where user_id=Users.id and alarm_id=Alarms.id and site_id=Sites.id");
  }

  function update($params)
  {
    $uElt = $this->getEntry($params);
    if(!$uElt)
      {
	// create new entry
	$this->newEntry(array_merge($params,
				    array( "last_mail_raised_date" => "",
					   "last_sms_raised_date" => "",
					   "nb_mail_raised" => 0,
					   "nb_sms_raised" => 0,
					   "last_sms_msg" => "",
					   "last_mail_msg" => "")));
	$uElt = $this->getEntry($params);
      }
    
    if($params["sendEmail"])
      {
	$uElt["last_mail_raised_date"] = $params["date"];
	$uElt["nb_mail_raised"] += 1;
	$uElt["last_mail_msg"] = $params["mail_msg"];
      }

    if($params["sendSMS"])
      {
	$uElt["last_sms_raised_date"] = $params["date"];
	$uElt["nb_sms_raised"] += 1;
	$uElt["last_sms_msg"] = $params["sms_msg"];
      }    

    $this->setEntry($uElt);
  }  
}

?>
