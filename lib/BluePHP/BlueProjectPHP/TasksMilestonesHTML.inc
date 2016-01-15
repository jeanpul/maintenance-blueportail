<?php

require_once("ModuleInterface.inc");
require_once("Projects.inc");
require_once("Tasks.inc");
require_once("Forms.inc");
require_once("TasksMilestones.inc");

class TasksMilestonesHTML extends ModuleInterface
{
  function TasksMilestonesHTML()
  {    
    parent::setName("TasksMilestones");
    parent::setModule(new TasksMilestones());
  }

  protected function getTableInput($params)
  {
    $str = "<table>\n";
    
    $module = new Projects();
    $str .= get2CellsRow("Suivis", getSelectTemplate2($module->getSuivis($params),
						       array( "value" => "project_name",
							      "name" => "project_id",
							      "key" => "id",
							      "default" => $params["project_id"],
							      "extra" => createJSCall("changeTasksProject", array()))));

    $module = new Tasks();
    $str .= get2CellsRow("Task", getSelectTemplate2($module->getEntries($params),
						    array( "value" => "tasks_name",
							   "name" => "tasks_id",
							   "key" => "tasks_id",
							   "default" => $params["tasks_id"],
							   "extra" => false )));

    $str .= get2CellsRow("Date", getInputDate("date", $params["date"]));

    $str .= get2CellsRow("Description", 
			 getInputTextArea2($params["action"] == "new" ? "" : $params["comments"],
					   array( "name" => "comments", 
						  "rows" => 4, 
						  "cols" => 40, 
						  "extra" => false )));

    $str .= "</table>\n";
    
    return $str;			 
  }
}

?>
