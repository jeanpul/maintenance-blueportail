<?php

include_once("BluePHP/Utils/ModuleProcessing.inc");

class PAXOps extends ModuleProcessing
{
  function PAXOps()
  {
    parent::setParams( array( "name" => "counters",
			      "keyname" => "sensorId",
			      "sort" => "clientId" ));

  }
}

?>
