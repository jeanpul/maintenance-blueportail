<?php

require_once("Forms.inc");
require_once("BluePHP/Input.inc");
require_once("BluePHP/ModuleInterface.inc");
require_once("Projects.inc");
require_once("Familly.inc");
require_once("Companies.inc");
require_once("Contacts.inc");

class ProjectsHTML extends ModuleInterface
{
  function ProjectsHTML($params)
  {    
    parent::setName(_("Projects"));
    parent::setModule(new Projects());
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
	$winTitle = _("Projects configuration");
	$winContent = "Projects"; 
      }
    else
      {    
	$winTitle = _("Projects configuration");
	$winContent = "ProjectsEdit"; 
      }
    parent::setTitle($winTitle);
    parent::setContent($winContent);
    parent::setParams($params);
  }

  protected function getTableInput($params)
  {
    $str = "<table>\n";

    $str .= get2CellsRow("Name", createInputText($params["project_name"],
						 array( "name" => "project_name", 
							"readonly" => false,
							"extra" => false)));

    $familly = new Familly();
    $fp = new FamillyProjects();
    $p = $fp->getEntry($params);
    if($p) 
      {
	$params["familly_id"] = $p["familly_id"];
      }
    else
      {    
	$params["familly_id"] = "";
      }
    $str .= get2CellsRow("Familly", createSelect($familly->getEntries(),
						 array( "value" => "familly_name",
							"name" => "familly_id",
							"key" => "familly_id",
							"default" => $params["familly_id"],
							"extra" => false )));
    
    $str .= get2CellsRow("Description", 
			 createInputTextArea($params["project_description"], 
					     array( "name" => "project_description",
						    "rows" => 4, 
						    "cols" => 40, 
						    "extra" => false )));
    
    $module = new Companies();
    $str .= get2CellsRow("Client", createSelect($module->getEntries(),
						array( "value" => "company_name",
						       "name" => "company_id",
						       "key" => "company_id",
						       "default" => $params["company_id"],
						       "extra" => false )));
    
    $str .= get2CellsRow("Start", 
			 createInputDate($params["project_datestart"], array( "name" => "project_datestart",
									      "title" => "cal",
									      "format" => "yyyy-MM-dd")));

    $str .= get2CellsRow("End", 
			 createInputDate($params["project_dateend"], array( "name" => "project_dateend", 
									    "title" => "cal",
									    "format" => "yyyy-MM-dd")));
    

    $module = new Contacts();
    $str .= get2CellsRow("Chef de projet", createSelect($module->getContactsFromCompany(array( "company_id" => 9 )),
							array( "value" => "contact_first_name,contact_last_name",
							       "name" => "project_manager",
							       "key" => "contact_id",
							       "default" => $params["project_manager"],
							       "extra" => false )));

    $str .= get2CellsRow("Technique", createSelect($module->getContactsFromCompany(array( "company_id" => 9 )),
						   array( "value" => "contact_first_name,contact_last_name",
							  "name" => "project_technical",
							  "key" => "contact_id",
							  "default" => $params["project_technical"],
							  "extra" => false )));
    
    $str .= get2CellsRow("Status", 
			 createSelect(array('none','AvantVente','Propale',
					    'Commande','Livraison','Facturation','Maintenance', 'Termine', 'err'),
				      array( "value" => false,
					     "name" => "project_status",
					     "key" => false,
					     "default" => $params["project_status"],
					     "extra" => false )));
    
    $str .= "</table>\n";
    
    return $str;			 
  }
}

?>