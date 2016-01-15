<?php

include_once("BlueProjectPHP/ModuleProcessing.inc");


class TasksMilestones extends ModuleProcessing
{
  function TasksMilestones()
  {
    parent::setParams(array( "name" => "tasks_milestones",
			     "keyname" => "tasks_id",
			     "sort" => "date desc" ));
  }
  
}

?>
