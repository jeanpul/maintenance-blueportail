<?php

require_once("ModuleInterface.inc");
require_once("Forms.inc");
require_once("Tasks.inc");

class TasksHTML extends ModuleInterface
{
  function TasksHTML()
  {    
    parent::setName("Tasks");
    parent::setModule(new Tasks());
  }

  protected function getTableInput($params)
  {
    $str = "<table>\n";
    
    $str .= get2CellsRow("Nom", getInputText2($params["tasks_name"],
					      array( "name" => "tasks_name",
						     "readonly" => false,
						     "extra" => false )));
    
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
