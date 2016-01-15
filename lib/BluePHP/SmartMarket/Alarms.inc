<?php

include_once("BluePHP/Utils/ModuleProcessing.inc");

class Alarms extends ModuleProcessing
{

  function Alarms()
  {
    parent::setParams( array( "name" => "Alarms",
			      "keyname" => "id",
			      "sort" => "name" ));
  }

  function getEntries()
  {
    return parent::Select("Alarms.id as id, Alarms.name as name, ttr, " .
			  "function_id, email, sms, Functions.id as fid, " .
			  "Functions.name as fname from Alarms, Functions " .
			  "where Alarms.function_id=Functions.id order by Alarms.name");
  }

  function getFunctions()
  { 
    return parent::Select("* from Functions");
  }

  function getFunction($params)
  {
    return parent::Select("Functions.id as id, Functions.name as name from " . 
			  "Alarms, Functions where Alarms.function_id=Functions.id and " 
			  . "Alarms.id="  . $params["id"]);
  }

  function getSMSText($params)
  {
    return parent::Select("sms from Alarms where Alarms.id=" . $params["id"]);
  }

  function getEmailText($params)
  {
    return parent::Select("email from Alarms where Alarms.id=" . $params["id"]);
  }

}

?>