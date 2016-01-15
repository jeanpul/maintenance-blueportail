<?php

include_once("BluePHP/BlueProjectPHP/Config.inc");
include_once("BluePHP/Utils/ModuleProcessing.inc");

class PortForward extends ModuleProcessing
{
  public function __construct($sharedCon = false)
  {
    parent::__construct(array( "name" => "portforward",
			       "keyname" => array( "project_id", "srcIP", "srcPort" ),
			       "sort" => "project_id" ));
  }
}

?>