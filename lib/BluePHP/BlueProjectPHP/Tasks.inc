<?php

include_once("BlueProjectPHP/ModuleProcessing.inc");


class Tasks extends ModuleProcessing
{
  function Tasks()
  {
    parent::setParams(array( "name" => "tasks",
			     "keyname" => "tasks_id",
			     "sort" => "tasks_name" ));
  }
}

?>
