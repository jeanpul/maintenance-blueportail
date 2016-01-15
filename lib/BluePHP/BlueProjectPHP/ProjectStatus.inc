<?php

include_once("BluePHP/ModuleProcessing.inc");
include_once("CommonOps.inc");

class ProjectStatus extends ModuleProcessing
{
  function ProjectStatus()
  {
    parent::setParams(array( "name" => "project_status",
			     "keyname" => "project_status_id",
			     "sort" => "date" ));
  }
}



?>
