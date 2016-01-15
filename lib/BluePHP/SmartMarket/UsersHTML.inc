<?php

require_once("BluePHP/Utils/ModuleInterface.inc");
require_once("Users.inc");

class UsersHTML extends ModuleInterface
{
  function UsersHTML($params)
  {
    parent::setName(_("Users"));
    parent::setModule(new Users());
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
	$winTitle = _("Users configuration");
	$winContent = "Users"; 
      }
    else
      {    
	$winTitle = _("Users configuration");
	$winContent = "UsersEdit"; 
      }
    parent::setTitle($winTitle);
    parent::setContent($winContent);
    parent::setParams($params);
  }

  function getTableInput($params)
  {
    $str = "<table>\n";    
    $str .= get2CellsRow(_("Name"), getInputText($params["name"],
					      array( "name" => "name",
						     "readonly" => false,
						     "extra" => false )));

    $str .= get2CellsRow(_("E-mail"), getInputText($params["email"],
						array( "name" => "email",
						       "readonly" => false,
						       "extra" => false )));

    $str .= get2CellsRow(_("Tel"), getInputText($params["tel"],
					     array( "name" => "tel",
						    "readonly" => false,
						    "extra" => false )));

    $str .= "</table>\n";
    return $str;
  }

  function getTableList($pagename)
  {
    return parent::getModuleTableList(array("ctitle" => _("Create a new User"), 
					    "titles" => array( _("Name"), _("E-mail"), _("Tel") ),
					    "cols" => array( "name", "email", "tel" ) ),
				      $pagename);
  }
  
}


?>