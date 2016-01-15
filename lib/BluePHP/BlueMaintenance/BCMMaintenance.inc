<?php

include_once("BluePHP/BlueMaintenance/BaseMaintenance.inc");
include_once("BluePHP/BlueMaintenance/WarningInsert.inc");

class BCMMaintenance extends BaseMaintenance {

  public function __construct() {
    parent::__construct(array("name" => "BCM",
	"keyname" => array("ID", "DateStart")));
  }
 
  public function runningTest($id,$ref,$license, $dateEnd) {
    $wobj = new WarningInsert(); 
    if ($license != "running") {
     $state = "Warning";	 
      $attributeName = 'WarningLicense';
      $wobj->updateWarning($id,$ref,$dateEnd,$attributeName);
    } 
    $state = $wobj->WhatState($id, $dateEnd);
    return $state;
  }
}

?>
