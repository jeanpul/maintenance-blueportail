<?php

require_once("BluePHP/Window.inc");
require_once("BluePHP/ModuleInterface.inc");
require_once("BluePHP/Input.inc");
require_once("Forms.inc");
require_once("CommonOps.inc");
require_once("VCS.inc");
require_once("Projects.inc");

class VCSHTML extends ModuleInterface
{
  function VCSHTML($params)
  {    
    parent::setName("VCS");
    parent::setModule(new VCS());
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
	$winTitle = _("VCS configuration");
	$winContent = "VCS"; 
      }
    else
      {    
	$winTitle = _("VCS configuration");
	$winContent = "VCSEdit"; 
      }
    parent::setTitle($winTitle);
    parent::setContent($winContent);
    parent::setParams($params);
  }

  protected function getTableInput($params)
  {
    $str = "<table>\n";

    $str .= get2CellsRow("ClientId (ref)", createInputText($params["ref"],
						array( "name" => "ref",
						       "readonly" => false,
						       "extra" => false )));

    $str .= get2CellsRow("MAC", getInputText($params["mac"],
					      array( "name" => "mac",
						     "readonly" => false,
						     "extra" => false )));

    
    $str .= get2CellsRow("Lot", getSelectTemplate($this->getModule()->getLots(),
						  array( "value" => "id",
							 "name" => "lot",
							 "key" => "id",
							 "default" => $params["lot"],
							 "extra" => false )));

    $str .= get2CellsRow("Product", getSelectTemplate($this->getModule()->getProducts(),
						      array( "value" => "ref",
							     "name" => "VCSP",
							     "key" => "ref",
							     "default" => $params["VCSP"],
							     "extra" => false )));

    $str .= get2CellsRow("Status", 
			 createSelect(array('new', 'test', 'ok', 'error', 'demoOnly', 'pret', 'client', '666'),
				      array( "value" => false,
					     "name" => "state",
					     "key" => false,
					     "default" => $params["state"],
					     "extra" => false )));


    $module = new Projects();
    $str .= get2CellsRow("Projet", getSelectTemplate($module->getEntries(),
						      array( "value" => "project_name",
							     "name" => "projet",
							     "key" => "project_id",
							     "default" => $params["projet"],
							     "extra" => false )));

    $str .= get2CellsRow("IP", createInputText($params["IP"],
					       array( "name" => "IP", 
						      "readonly" => false,
						      "extra" => false )));

    $str .= get2CellsRow("Comment", createInputTextArea($params["comment"], 
							array( "name" => "comment",
							       "rows" => 4, 
							       "cols" => 40, 
							       "extra" => false )));

    $str .= "</table>\n";
    
    return $str;			 
  }
}

?>
