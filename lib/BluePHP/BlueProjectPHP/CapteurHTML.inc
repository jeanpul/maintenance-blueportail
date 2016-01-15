<?php

require_once("Forms.inc");
require_once("BluePHP/Input.inc");
require_once("BluePHP/ModuleInterface.inc");
require_once("Capteur.inc");
require_once("Projects.inc");

class CapteurHTML extends ModuleInterface
{
  function CapteurHTML($params)
  {
    parent::setName(_("Capteur"));
    parent::setModule(new Capteur());
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
	$winTitle = _("Capteur configuration");
	$winContent = "Capteur"; 
      }
    else
      {    
	$winTitle = _("Capteur configuration");
	$winContent = "CapteurEdit"; 
      }
    parent::setTitle($winTitle);
    parent::setContent($winContent);
    parent::setParams($params);
  }


  protected function getTableInput($params)
  {
    $str = "<table>\n";
    
    $str .= get2CellsRow("MAC", getInputText($params["mac"],
					      array( "name" => "mac",
						     "readonly" => false,
						     "extra" => false )));
    $module = new Projects();
    $str .= get2CellsRow("Projet", getSelectTemplate($module->getEntries(),
						      array( "value" => "project_name",
							     "name" => "project_id",
							     "key" => "project_id",
							     "default" => $params["project_id"],
							     "extra" => false )));

    $str .= get2CellsRow("Ref", getSelectTemplate($this->getModule()->getProducts(),
						  array( "value" => "ref",
							 "name" => "product_ref",
							 "key" => "ref",
							 "default" => $params["product_ref"],
							 "extra" => false )));

    $str .= get2CellsRow("IP", createInputText($params["IP"],
					       array( "name" => "IP", 
						      "readonly" => false,
						      "extra" => false )));

    $str .= get2CellsRow("Comment", createInputTextArea($params["comments"], 
							array( "name" => "comments",
							       "rows" => 4, 
							       "cols" => 40, 
							       "extra" => false )));

    $str .="</table>\n";
    
    return $str;
  }

  function getTableList($pagename)
  {
    return parent::getModuleTableList(array("ctitle" => _("Create an new Capteur"),
					    "titles" => array( _("MAC"), _("Project") ),
					    "cols" => array( "mac", "project_id" ) ),
				      $pagename);
  }

}

?>
