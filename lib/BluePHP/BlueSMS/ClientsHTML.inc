<?php

require_once("BluePHP/Utils/ModuleInterface.inc");
require_once("BluePHP/Utils/Input.inc");
require_once("Clients.inc");

class ClientsHTML extends ModuleInterface
{
  function ClientsHTML($params)
  {
    parent::setName(_("Clients"));
    parent::setModule(new Clients());
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
	$winTitle = _("Clients configuration");
	$winContent = "Clients"; 
      }
    else
      {    
	$winTitle = _("Clients configuration");
	$winContent = "ClientsEdit"; 
      }
    parent::setTitle($winTitle);
    parent::setContent($winContent);
    parent::setParams($params);
  }

  function getTableInput($params)
  {
    $str = "<table>\n";    
    $str .= createTableRow(array( _("Id"),
				  createInputText($params["id"],
						  array( "name" => "id",
							 "readonly" => false,
							 "extra" => false ))
				  )
			   );

    $str .= createTableRow(array( _("API"),
				  createSelect( $this->getModule()->getFunctions(),
						array( "key" => "id",
						       "name" => "function_id",
						       "value" => "type,type_config",
						       "default" => $params["function_id"],
						       "extra" => false ))
				  )
			   );

    $str .= createTableRow(array( _("Comments"),
				  createInputTextArea($params["comment"],
						      array( "name" => "comment",
							     "rows" => 10,
							     "cols" => 40,
							     "extra" => false ))));
    $str .= "</table>\n";
    return $str;
  }

  function getTableList($pagename)
  {
    return parent::getModuleTableList(array("ctitle" => _("Create a new Client"), 
					    "titles" => array( _("Id"), _("API"), _("Comments") ),
					    "cols" => array( "id", "function_id", "comment" ) ),
				      $pagename);
  }
  
}


?>