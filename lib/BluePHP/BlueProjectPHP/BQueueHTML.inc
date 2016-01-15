<?php

require_once("Forms.inc");
require_once("BluePHP/Input.inc");
require_once("BluePHP/ModuleInterface.inc");
require_once("BQueue.inc");
require_once("Projects.inc");

class BQueueHTML extends ModuleInterface
{
  function BQueueHTML($params)
  {
    parent::setName(_("BQueue"));
    parent::setModule(new BQueue());
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
	$winTitle = _("BQueue configuration");
	$winContent = "BQueue"; 
      }
    else
      {    
	$winTitle = _("BQueue configuration");
	$winContent = "BQueueEdit"; 
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
    return parent::getModuleTableList(array("ctitle" => _("Create an new BQueue"),
					    "titles" => array( _("MAC"), _("Project") ),
					    "cols" => array( "mac", "project_id" ) ),
				      $pagename);
  }

}

?>
