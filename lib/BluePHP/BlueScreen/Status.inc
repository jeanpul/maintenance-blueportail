<?php

include_once("BluePHP/Utils/ModuleProcessing.inc");

class Status extends ModuleProcessing
{
  function Status()
  {
    parent::setParams( array( "name" => "Status",
			      "keyname" => "id",
			      "sort" => "id" ));

  }
}

?>

