<?php

require_once("BluePHP/Window.inc");
require_once("BluePHP/ModuleInterface.inc");
require_once("BluePHP/Input.inc");
require_once("Forms.inc");
require_once("CommonOps.inc");
require_once("Journal.inc");
require_once("Contacts.inc");
require_once("Companies.inc");
require_once("Projects.inc");

class JournalHTML extends ModuleInterface
{
  function JournalHTML($params)
  {    
    parent::setName("Journal");
    parent::setModule(new Journal());
    $this->setWindow($params);
  }

  function setWindow($params)
  {
    $params = array_merge( array( "action" => "list"), 
			   $params );

    $winTitle = "";
    $winContent = "";
    if($params["action"] == "list")
      {
	$winTitle = _("Journal configuration");
	$winContent = "Journal"; 
      }
    else
      {    
	$winTitle = _("Journal configuration");
	$winContent = "JournalEdit"; 
      }
    parent::setTitle($winTitle);
    parent::setContent($winContent);
    parent::setParams($params);
  }

  protected function getTableInput($params)
  {
    $str = "<table>\n";
    
    $module = new Projects();
    $str .= get2CellsRow("Projet", getSelectTemplate($module->getEntries(),
						       array( "value" => "project_name",
							      "name" => $module->getKey(),
							      "key" => $module->getKey(),
							      "default" => $params[$module->getKey()],
							      "extra" => false )));    

    $module = new Contacts();
    $str .= get2CellsRow("Employee", getSelectTemplate($module->getEmployees(),
						       array( "value" => "contact_first_name,contact_last_name",
							      "name" => $module->getKey(),
							      "key" => $module->getKey(),
							      "default" => $params[$module->getKey()],
							      "extra" => false )));    


    $str .= get2CellsRow("Date", getInputDate("journal_date", $params["journal_date"]));


    $str .= get2CellsRow("Type", createSelect(array( "email", "telephone", "confcall", "reunion", "none" ),
					      array( "value" => false,
						     "name" => "type",
						     "key" => false,
						     "default" => $params["type"],
						     "extra" => false )));    


    $str .= get2CellsRow("Description", 
			 getInputTextArea($params["action"] == "new" ? "" : $params["journal_description"],
					  array( "name" => "journal_description", 
						 "rows" => 4, 
						 "cols" => 40, 
						 "extra" => false )));

    $str .= "</table>\n";
    
    return $str;			 
  }
}

?>
