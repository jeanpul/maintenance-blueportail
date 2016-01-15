<?php

include_once("BlueProjectPHP/ModuleProcessing.inc");


class TasksRunning extends ModuleProcessing
{
  function TasksRunning()
  {
    parent::setParams(array( "name" => "tasks_running",
			     "keyname" => "tasks_id",
			     "sort" => "start desc" ));
  }
  
}

?>
