<?php

include_once("BluePHP/Utils/ModuleProcessing.inc");

class FamillyProjects extends ModuleProcessing
{
  public function __construct($sharedCon = false)
  {
    parent::__construct(array( "name" => "familly_projects",
			       "keyname" => "project_id",
			       "sort" => "familly_id" ),
			$sharedCon);
  }
}


?>
