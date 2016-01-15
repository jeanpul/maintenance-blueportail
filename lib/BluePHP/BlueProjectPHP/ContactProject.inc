<?php

include_once("BluePHP/ModuleProcessing.inc");


class ContactProject extends ModuleProcessing
{
  function ContactProject()
  {
    parent::setParams(array( "name" => "contacts_projects",
			     "keyname" => "contacts_projects_id" ));

  }
}

?>
