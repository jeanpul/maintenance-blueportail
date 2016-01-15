<?php

include_once("BluePHP/BlueMaintenance/WarningInsert.inc");
include_once("BluePHP/BlueMaintenance/BaseMaintenance.inc");

class BTOPMaintenance extends BaseMaintenance {

  public function __construct() {
    parent::__construct(array("name" => "BTOP",
			      "keyname" => array("ID", "DateStart")));  
  }
  
  public function runningTest($id, $ref, $license, $sensors, $dateEnd) {
    $wobj = new WarningInsert(); 
    if ($license != "running") {	 
      $attributeName = 'WarningLicense';     
      $wobj->updateWarning($id,$ref,$dateEnd,$attributeName);
    } 
    if ($sensors != "running") {	 
      $attributeName = 'WarningSensor';     
      $wobj->updateWarning($id,$ref,$dateEnd,$attributeName);
    } 
    $state = $wobj->WhatState($id, $dateEnd); 
    return $state;
  }
  
}

?>
