<?php

require_once("BluePHP/Utils/ModuleInterface.inc");
require_once("BluePHP/Utils/Input.inc");
require_once("Screens.inc");

class ScreensHTML extends ModuleInterface
{
  function ScreensHTML($params)
  {
    parent::setName(_("Screens"));
    parent::setModule(new Screens());
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
	$winTitle = _("Screens configuration");
	$winContent = "Screens"; 
      }
    else
      {    
	$winTitle = _("Screens configuration");
	$winContent = "ScreensEdit"; 
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

    $str .= createTableRow(array( _("IP"),
				  createInputText($params["ip"],
						  array( "name" => "ip",
							 "readonly" => false,
							 "extra" => false ))
				  )
			   );

    $str .= createTableRow(array( _("Logic"),
				  createSelect( $this->getModule()->getLogics(),
						array( "key" => "id",
						       "name" => "logic_id",
						       "value" => "id",
						       "default" => $params["logic_id"],
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
    return parent::getModuleTableList(array("ctitle" => _("Create a new Screen"), 
					    "titles" => array( _("Id"), _("ip"), _("Logic"), _("Comment") ),
					    "cols" => array( "id", "ip", "logic_id", "comment" ) ),
				      $pagename);
  }
  
}


?>