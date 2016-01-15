<?php

require_once("Forms.inc");
require_once("BluePHP/Input.inc");
require_once("BluePHP/ModuleInterface.inc");
require_once("ProjectStatus.inc");
require_once("Projects.inc");

class ProjectStatusHTML extends ModuleInterface
{
  function ProjectStatusHTML($params)
  {    
    parent::setName(_("ProjectStatus"));
    parent::setModule(new ProjectStatus());
    $this->setWindow($params);

    parent::addLabel("project_id", "Project");
    parent::addLabel("date", "Date");
    parent::addLabel("who", "Who");
    parent::addLabel("status", "Status");
    parent::addLabel("comments", "Description");
  }
  
  function setWindow($params)
  {
    $params = array_merge( array( "action" => "list"), 
			   $params );

    $winTitle = "";
    $winContent = "";
    if($params["action"] == "list")
      {
	$winTitle = _("Project status configuration");
	$winContent = "ProjectStatus"; 
      }
    else
      {    
	$winTitle = _("Project status modification");
	$winContent = "ProjectStatusEdit"; 
      }
    parent::setTitle($winTitle);
    parent::setContent($winContent);
    parent::setParams($params);
  }

  function render_project($params)
  {
    $module = new Projects();
    return createTableRow(array(parent::getLabel("project_id"), 
				createSelect($module->getEntries(),
					     array( "value" => "project_name",
						    "name" => $module->getKey(),
						    "key" => $module->getKey(),
						    "default" => $params[$module->getKey()],
						    "extra" => false ))));
  }

//  protected function getTableInput($params)
//  {
//    $str = "<table>\n";
//
//    $module = new Projects();
//
//    $str .= createTableRow(array("Projet", 
//				 createSelect($module->getEntries(),
//					      array( "value" => "project_name",
//						     "name" => $module->getKey(),
//						     "key" => $module->getKey(),
//						     "default" => $params[$module->getKey()],
//						     "extra" => false ))));
//
//    $str .= get2CellsRow("Date", getInputDate("date", $params["date"]));
//
//
//    $str .= get2CellsRow("Status", 
//			 createSelect(array('OK', 'Customer', 'BEV'),
//				      array( "value" => false,
//					     "name" => "status",
//					     "key" => false,
//					     "default" => $params["action"] == "new" ? "OK" : $params["status"],
//					     "extra" => false )));
//    
//    $str .= get2CellsRow("Description", 
//			 getInputTextArea($params["action"] == "new" ? "" : $params["comments"],
//					  array( "name" => "comments", 
//						 "rows" => 4, 
//						 "cols" => 40, 
//						 "extra" => false )));
//    $str .= "</table>\n";    
//    return $str;			 
//  }

  function getTableList($pagename)
  {
    return parent::getModuleTableList(array("ctitle" => _("Create a new Project status"), 
					    "titles" => array( _("Project Id"), _("date"), _("Status"), _("Comments") ),
					    "cols" => array( "project_id", "date", "status", "comments" ) ),
				      $pagename);
  }
}

?>