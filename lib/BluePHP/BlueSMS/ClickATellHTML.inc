<?php

include_once("BluePHP/Utils/ModuleInterface.inc");
include_once("BluePHP/Utils/Input.inc");
include_once("sms/ClickATell.inc");

class ClickATellHTML extends ModuleInterface
{
  function ClickATellHTML($params)
  {
    parent::setName(_("ClickATell"));
    parent::setModule(new ClickATell());
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
	$winTitle = _("ClickATell configuration");
	$winContent = "ClickATell"; 
      }
    else
      {    
	$winTitle = _("ClickATell configuration");
	$winContent = "ClickATellEdit"; 
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

    $str .= createTableRow(array( _("User"),
				  createInputText($params["user"],
						  array( "name" => "user",
							 "readonly" => false,
							 "extra" => false ))
				  )
			   );

    $str .= createTableRow(array( _("Password"),
				  createInputText($params["password"],
						  array( "name" => "password",
							 "readonly" => false,
							 "extra" => false ))
				  )
			   );

    $str .= createTableRow(array( _("API Id"),
				  createInputText($params["api_id"],
						  array( "name" => "api_id",
							 "readonly" => false,
							 "extra" => false ))
				  )
			   );

    $str .= "</table>\n";
    return $str;
  }

  function getTableList($pagename)
  {
    return parent::getModuleTableList(array("ctitle" => _("Create a new ClickATell account"), 
					    "titles" => array( _("Id"), _("User"), _("Password"), _("Api Id") ),
					    "cols" => array( "id", "user", "password", "api_id" ) ),
				      $pagename);
  }
  
}


?>