<?php

include_once("BluePHP/BlueMaintenance/BaseMaintenance.inc");
include_once("BluePHP/BlueMaintenance/WarningInsert.inc");

class BQueueMaintenance extends BaseMaintenance {

  public function __construct() {
    parent::__construct(array("name" => "BQueue",
	"keyname" => array("ID", "DateStart")));
  }
  
  public function setSensorWarning($params) {
    $params = array_merge( array( "Terminated" => 0,
				  "dateEnd" => strftime("%Y-%m-%d %H:%M:%S") ), $params );
    $wobj = new WarningInsert();
    $lastEntry = $wobj->LastEntry($params["ID"], $params["type"]);

    if($params["Terminated"] == 1 and !is_null($lastEntry))
      {
	$this->QueryNoReturn("update AdvancedWarning set Terminated=\"1\" where ID=\"" . $params["ID"] . 
			     "\" and Terminated=\"0\" and AttributeName=\"" . 
			     $params["type"] . "\"");
      } else 
      {
	$wobj->updateWarning($params["ID"], $params["Ref"], $params["dateEnd"], $params["type"]);
      }

    $state = $wobj->WhatState($params["ID"], $params["dateEnd"]);

    $lastState = $this->getLastStateFromId($params["ID"]);    
    if($lastState)
      {
	if($state != $lastState["Etat"])
	  {
	    if (($lastState['Etat'] == "WarningChecked" and $state =="Warning") or 
		($lastState['Etat'] == "Checked" and $state =="OK")) 
	      {
		$this->setEntry( array_merge( $lastState, array("DateEnd" => $params["dateEnd"]) ) );
	      } 
	    else 
	      {
		$this->newEntry(array_merge( $lastState , array( "Etat" => $state, 
								 "DateStart" => $params["dateEnd"], 
								 "DateEnd" => $params["dateEnd"] ) ) );
	      }
	  }
      }
    return $wobj->LastEntry($params["ID"], $params["type"]);
  }

  public function runningTest($id,$ref,$license, $sensors, $servers, $dateEnd) {
    $wobj = new WarningInsert();
    $resL = $wobj->LastEntry($id,"WarningLicense");
    $resSen= $wobj->LastEntry($id,"WarningSensor");
    $resSer= $wobj->LastEntry($id,"WarningServer");
    
    if ($license != "running") {  
      $attributeName = 'WarningLicense';
      $wobj->updateWarning($id,$ref,$dateEnd,$attributeName);
    } else if ($resL != null ) {
      $this->QueryNoReturn("update AdvancedWarning set Terminated=\"1\" where ID=\"$id\" and Terminated=\"0\" and AttributeName=\"WarningLicense\"");
    }

    if ($sensors != "running") {
      $attributeName = 'WarningSensor';
      $wobj->updateWarning($id,$ref,$dateEnd,$attributeName);
    }  else if ($resSen != null ) {
      $this->QueryNoReturn("update AdvancedWarning set Terminated=\"1\" where ID=\"$id\" and Terminated=\"0\" and AttributeName=\"WarningSensor\"");
    }

    if ($servers != "running") {
      $attributeName = 'WarningServer';
      $wobj->updateWarning($id,$ref,$dateEnd,$attributeName);
    }else if ($resSer != null ) {
      $this->QueryNoReturn("update AdvancedWarning set Terminated=\"1\" where ID=\"$id\" and Terminated=\"0\" and AttributeName=\"WarningServer\"");
    }
   
    $state = $wobj->WhatState($id, $dateEnd); 
    return $state;
  }
}
?>