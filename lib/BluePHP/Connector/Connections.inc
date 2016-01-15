<?php

include_once("BluePHP/Utils/ModuleProcessing.inc");

class Connections extends ModuleProcessing
{
  public function __construct()
  {
    parent::__construct( array( "name" => "Connections",
				"keyname" => array( "project_id", "host", "type" ) ) );
    parent::setConParams( array( "type" => "SQLITE3",
				 "host" => "localhost",
				 "name" => "/home/DATA/Connector/connectors.db",
				 "user" => null,
				 "passwd" => null ) );
  }
}

?>