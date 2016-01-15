<?php

include_once("BluePHP/Utils/ModuleProcessing.inc");
include_once("BluePHP/BTopLocalServer/Config.inc");

class Tasks extends ModuleProcessing
{
  public function __construct($sharedCon = null)
  {
    parent::setParams( array( "name" => "tasks",
			      "keyname" => "id",
			      "sort" => "id" ),
		       $sharedCon);    
    $this->setConParams( array( "type" => DATADBTYPE,
				"host" => DATADBHOST,
				"name" => BTOP_SERVER_DBPATH . "task.db",
				"user" => DATADBUSER,
				"passwd" => DATADBPASSWD ) );
  }

  public function getNonProcessedTasksPrev()
  {
    return parent::Select("* from tasks_prev where isProcessed=0");
  }
}

?>