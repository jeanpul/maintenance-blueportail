<?php

require_once("BluePHP/Utils/ModuleInterface.inc");
require_once("Sites.inc");

class SitesHTML extends ModuleInterface
{
  function SitesHTML($params)
  {
    parent::setName(_("Sites"));
    parent::setModule(new Sites());
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
	$winTitle = _("Sites configuration");
	$winContent = "Sites"; 
      }
    else
      {    
	$winTitle = _("Sites configuration");
	$winContent = "SitesEdit"; 
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

    $str .= get2CellsRow(_("BlueCount server"), getInputText($params["server"],
						array( "name" => "server",
						       "readonly" => false,
						       "extra" => false )));

    $str .= get2CellsRow(_("BluePortail client"), getInputText($params["blueportail"],
							       array( "name" => "blueportail",
								      "readonly" => false,
								      "extra" => false )));

    $str .= "</table>\n";
    return $str;
  }

  function getTableList($pagename)
  {
    return parent::getModuleTableList(array("ctitle" => _("Create a new Site"), 
					    "titles" => array( _("Name"), _("Server") ),
					    "cols" => array( "name", "server" ) ),
				      $pagename);
  }
  
}


?>