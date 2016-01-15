<?php

require_once("Forms.inc");
require_once("BluePHP/Input.inc");
require_once("BluePHP/ModuleInterface.inc");
require_once("Projects.inc");
require_once("BlueCountGUI.inc");

class BlueCountGUIHTML extends ModuleInterface
{
  function BlueCountGUIHTML($params)
  {
    parent::setName(_("BlueCountGUI"));
    parent::setModule(new BlueCountGUI());
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
	$winTitle = _("BlueCountGUI configuration");
	$winContent = "BlueCountGUI"; 
      }
    else
      {    
	$winTitle = _("BlueCountGUI configuration");
	$winContent = "BlueCountGUIEdit"; 
      }
    parent::setTitle($winTitle);
    parent::setContent($winContent);
    parent::setParams($params);
  }

  protected function getTableInput($params)
  {
    $str = "<table>\n";
    
    $str .= get2CellsRow("mac", createInputText($params["mac"],
						array( "name" => "mac",
						       "readonly" => false,
						       "extra" => false )));

    $module = new Projects();
    $str .= get2CellsRow("Project", createSelect($module->getEntries(),
						 array( "value" => "project_name", 
							"name" => "project_id", 
							"key" => $module->getKey(), 
							"default" => $params[$module->getKey()], 
							"extra" => false )));
    
    
    $str .= get2CellsRow("Ref", createSelect($this->getModule()->getProducts(),
					     array( "value" => "ref", 
						    "name" => "product_ref", 
						    "key" => "ref", 
						    "default" => $params["product_ref"], 
						    "extra" => false )));

    $str .= get2CellsRow("IP", createInputText($params["IP"],
					       array( "name" => "IP", 
						      "readonly" => false,
						      "extra" => false )));

    $str .= get2CellsRow("Adresse", createInputTextArea($params["comments"], 
							array( "name" => "comments",
							       "rows" => 4, 
							       "cols" => 40, 
							       "extra" => false )));
    $str .="</table>\n";
    
    return $str;
  }

  function getTableList($pagename)
  {
    return parent::getModuleTableList(array("ctitle" => _("Create an new BlueCountGUI"),
					    "titles" => array( _("MAC"), _("Project"), _("Ref") ),
					    "cols" => array( "mac", "project_id", "product_ref" )),
					    $pagename);
  }

}

?>
