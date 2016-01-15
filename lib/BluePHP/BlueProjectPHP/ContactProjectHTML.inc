<?php

require_once("BluePHP/Input.inc");
require_once("BluePHP/ModuleInterface.inc");
require_once("ContactProject.inc");
require_once("Companies.inc");
require_once("Projects.inc");
require_once("Contacts.inc");

class ContactProjectHTML extends ModuleInterface
{
  function ContactProjectHTML($params)
  {    
    parent::setName(_("ContactProject"));
    parent::setModule(new ContactProject());
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
	$winTitle = _("Contact Project configuration");
	$winContent = "ContactProject"; 
      }
    else
      {    
	$winTitle = _("Contact Project configuration");
	$winContent = "ContactProjectEdit"; 
      }
    parent::setTitle($winTitle);
    parent::setContent($winContent);
    parent::setParams($params);
  }

  protected function getTableInput($params)
  {
    $str = "<table>\n";
    
    $module = new Projects();
    $str .= createTableRow(array("Projet", 
				 createSelect($module->getEntries(),
					      array( "value" => "project_name",
						     "name" => $module->getKey(),
						     "key" => $module->getKey(),
						     "default" => $params[$module->getKey()],
						     "extra" => false ))));

    $module = new Contacts();
    $contact = $module->getEntry(array( "contact_id" => $params["contact_id"]));    

    $module = new Companies();
    $str .= createTableRow(array("Companie", 
				 createSelect($module->getEntries(),
					      array( "value" => "company_name",
						     "name" => $module->getKey(),
						     "key" => $module->getKey(),
						     "default" => $contact["company_id"],
						     "extra" => createAction("changeContactsFromCompany", array())))));   

    $module = new Contacts();
    $str .= createTableRow(array("Contact", 
				 createSelect($module->getContactsFromCompany($contact),
					      array ("value" => "contact_first_name,contact_last_name",
						     "name" => $module->getKey(), 
						     "key" => $module->getKey(),
						     "default" => $params[$module->getKey()],
						     "extra" => false ))));   

    $str .= createTableRow(array("Titre", 
				 createInputText($params["title"],
						 array( "name" => "title", 
							"readonly" => false,
							"extra" => false))));
    
    $str .= get2CellsRow("Description", 
			 createInputTextArea($params["comments"], 
					     array( "name" => "comments",
						    "rows" => 4, 
						    "cols" => 40, 
						    "extra" => false )));
    $str .= "</table>\n";
    
    return $str;			 
  }

  function getTableList($pagename)
  {
    return parent::getModuleTableList(array("ctitle" => _("Create an new Contact Project"),
					    "titles" => array( _("FirstName"), _("LastName") ),
					    "cols" => array( "contact_first_name", "contact_last_name" ) ),
				      $pagename);
  }

}

?>
