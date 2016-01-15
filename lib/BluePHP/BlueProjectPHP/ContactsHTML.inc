<?php

require_once("Forms.inc");
require_once("BluePHP/Input.inc");
require_once("BluePHP/ModuleInterface.inc");
require_once("Companies.inc");
require_once("Contacts.inc");

class ContactsHTML extends ModuleInterface
{
  function ContactsHTML($params)
  {
    parent::setName(_("Contacts"));
    parent::setModule(new Contacts());
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
	$winTitle = _("Contacts configuration");
	$winContent = "Contacts"; 
      }
    else
      {    
	$winTitle = _("Contacts configuration");
	$winContent = "ContactsEdit"; 
      }
    parent::setTitle($winTitle);
    parent::setContent($winContent);
    parent::setParams($params);
  }

  protected function getTableInput($params)
  {
    $str = "<table>\n";
    
    $str .= get2CellsRow("Nom", createInputText($params["contact_last_name"],
						array( "name" => "contact_last_name",
						       "readonly" => false,
						       "extra" => false )));

    $str .= get2CellsRow("Prénom", createInputText($params["contact_first_name"],
						   array( "name" => "contact_first_name", 
							  "readonly" => false,
							  "extra" => false )));

    $module = new Companies();
    $str .= get2CellsRow("Companie", createSelect($module->getEntries(),
						  array( "value" => "company_name", 
							 "name" => $module->getKey(), 
							 "key" => $module->getKey(), 
							 "default" => $params[$module->getKey()], 
							 "extra" => false )));

    $str .= get2CellsRow("Email", createInputText($params["contact_email"],
						  array( "name" => "contact_email", 
							 "readonly" => false,
							 "extra" => false )));

    $str .= get2CellsRow("Téléphone", createInputText($params["contact_phone"],
						      array( "name" => "contact_phone",
							     "readonly" => false,
							     "extra" => false )));

    $str .= get2CellsRow("Portable", createInputText($params["contact_mobile"],
						     array( "name" => "contact_mobile",
							    "readonly" => false,
							    "extra" => false )));

    $str .= get2CellsRow("Adresse", createInputTextArea($params["contact_address"], 
							array( "name" => "contact_address",
							       "rows" => 4, 
							       "cols" => 40, 
							       "extra" => false )));

    $str .= get2CellsRow("Sîte web", createInputText($params["contact_website"],
						     array( "name" => "contact_website", 
							    "readonly" => false,
							    "extra" => false )));
    $str .= get2CellsRow("Fax", createInputText($params["contact_fax"], 
						array( "name" => "contact_fax", 
						       "readonly" => false,
						       "extra" => false )));
    $str .="</table>\n";
    
    return $str;
  }

  function getTableList($pagename)
  {
    return parent::getModuleTableList(array("ctitle" => _("Create an new Contact"),
					    "titles" => array( _("FirstName"), _("LastName") ),
					    "cols" => array( "contact_first_name", "contact_last_name" ) ),
				      $pagename);
  }

}

?>
