<?php

include_once("BluePHP/Utils/ModuleProcessing.inc");

class AlarmsSites extends ModuleProcessing
{
  function AlarmsSites()
  {
    parent::setParams( array( "name" => "AlarmsSites",
			      "keyname" => "alarm_id,site_id" ));
  }

  function getEntries()
  {    
    $res = parent::Select("Alarms.name as aname, Sites.name as sname, " .
			  "alarm_id, site_id, comment from " . 
			  "Alarms, AlarmsSites, Sites " .
			  "where AlarmsSites.alarm_id=Alarms.id and " .
			  "AlarmsSites.site_id=Sites.id");
    return $res;
  }
}

?>
