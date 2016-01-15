<?php

include_once("BluePHP/BlueProjectPHP/Config.inc");
include_once("BluePHP/Utils/ModuleProcessing.inc");

class Contacts extends ModuleProcessing
{
  function Contacts()
  {
    parent::setParams(array( "name" => "contacts",
			     "keyname" => "contact_id",
			     "sort" => "contact_last_name" ));
  }

  // TODO fab: utiliser from company
  // et faire plutot une methode getEmployeesAction
  function getEmployees()
  {
    return parent::Select("contact_first_name, contact_last_name, contact_id " . 
			  "from contacts natural join action_contact");
  }

  function getEmployeesCommercial()
  {
    return parent::Select("contact_first_name, contact_last_name, com_contact.contact_id " . 
			  "from com_contact natural join contacts");
  }

  function getEmployeesFromProjects($params)
  {
    return parent::Select("contact_first_name, contact_last_name, contacts.contact_id as contact_id " . 
			  "from contacts, contacts_projects " . 
			  "where project_id=" . $params["project_id"] . " and contacts.contact_id=contacts_projects.contact_id " .
			  " and company_id=9 order by contact_last_name, contact_first_name");
  }

  function getContactsFromCompany($params)
  {
    $where = $params["company_id"] ? "where company_id=" . $params["company_id"] : "";
    return parent::Select("contact_first_name, contact_last_name, contact_id " . 
			  "from contacts $where order by contact_last_name, contact_first_name");
  }

  function getEntry($params)
  {
    if($params["contact_id"])
      {
	return parent::getEntry($params);
      }
    else
      {
	$contact = parent::Select("* from contacts order by contact_last_name, contact_first_name limit 1");
	return $contact[0];
      }
  }
}

?>
