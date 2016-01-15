<?php

include_once("BluePHP/ModuleProcessing.inc");


class ManualValidation extends ModuleProcessing
{
  function ManualValidation()
  {
    parent::setParams(array( "name" => "manual_validation",
			     "keyname" => "manual_validation_id",
			     "sort" => "datestart",
			     "links" => array( "contact_id" => array( "table" => "contacts", "key" => "contact_id", "name" => "contact_first_name" ) )));

  }
}


?>