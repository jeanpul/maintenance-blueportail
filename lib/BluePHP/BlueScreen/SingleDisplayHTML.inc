<?php

require_once("BluePHP/Utils/ModuleInterface.inc");
require_once("BluePHP/Utils/Input.inc");
require_once("logics/SingleDisplay.inc");

class SingleDisplayHTML extends ModuleInterface
{
  function SingleDisplayHTML($params)
  {
    parent::setName(_("SingleDisplay"));
    parent::setModule(new SingleDisplay());
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
	$winTitle = _("SingleDisplay configuration");
	$winContent = "SingleDisplay"; 
      }
    else
      {    
	$winTitle = _("SingleDisplay configuration");
	$winContent = "SingleDisplayEdit"; 
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

    $str .= createTableRow(array( _("Config"),
				  createSelect( $this->getModule()->getConfigs(),
						array( "key" => "id",
						       "name" => "config",
						       "value" => "id",
						       "default" => $params["config"],
						       "extra" => false ))
				  )
			   );

    $str .= "</table>\n";
    return $str;
  }

  function getTableList($pagename)
  {
    return parent::getModuleTableList(array("ctitle" => _("Create a new Single Display"), 
					    "titles" => array( _("Id"), _("Config") ),
					    "cols" => array( "id", "config" ) ),
				      $pagename);
  }
  
}


?>