<?php

include_once("BluePHP/Utils/ModuleProcessing.inc");
require_once("Alarms.inc");

class AlarmsStatus extends ModuleProcessing
{

  function AlarmsStatus()
  {
    parent::setParams( array( "name" => "AlarmsStatus",
			      "keyname" => "alarm_id,site_id,site_entity_id",
			      "sort" => "alarm_id" ));
  }

  function getEntries()
  {
    return parent::Select("alarm_id, site_id, site_entity_id, AlarmsStatus.ttr as ttr," .
			  "Alarms.name as aname, Sites.name as sname, " .
			  "last_ttr, last_raised, nb_raised from AlarmsStatus, Alarms, Sites " .
			  "where alarm_id=Alarms.id and site_id=Sites.id");
  }

  function check($params)
  {
    $aModule = new Alarms();
    $aElt = $aModule->getEntry(array( "id" => $params["alarm_id"] ));

    $sElt = $this->getEntry($params);

    if(!$sElt)
      {
	// create new entry
	$this->newEntry(array_merge($params,
				    array( "ttr" => $aElt["ttr"],
					   "last_ttr" => $params["date"],
					   "last_raised" => "",
					   "nb_raised" => 0)));
	$sElt = $this->getEntry($params);
      }

    $alarmRaised = false;
    $isNewCheck = strtotime($sElt["last_ttr"]) < strtotime($params["date"]);
    if($isNewCheck)
      {
	if($params["cond"])
	  {	    
	    if($sElt["ttr"] > 0)
	      {
		$sElt["ttr"] -= 1;

		if(!$sElt["ttr"])
		  {
		    $sElt["nb_raised"] += 1;
		    $sElt["last_raised"] = $params["date"];
		    $alarmRaised = true;		
		  }
	      }
	  }
	else
	  {
	    $sElt["ttr"] = $aElt["ttr"];
	  }
	$sElt["last_ttr"] = $params["date"];
	$this->setEntry($sElt);
      }
    return $alarmRaised;
  }  
}

?>
