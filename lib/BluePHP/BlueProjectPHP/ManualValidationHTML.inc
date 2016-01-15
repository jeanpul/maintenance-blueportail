<?php

require_once("Forms.inc");
require_once("BluePHP/Input.inc");
require_once("BluePHP/ModuleInterface.inc");
require_once("ManualValidation.inc");

class ManualValidationHTML extends ModuleInterface
{
  function ManualValidationHTML($params)
  {    
    parent::setName(_("ManualValidation"));
    parent::setModule(new ManualValidation());
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
	$winTitle = _("ManualValidation configuration");
	$winContent = "ManualValidation"; 
      }
    else
      {    
	$winTitle = _("ManualValidation configuration");
	$winContent = "ManualValidationEdit"; 
      }
    parent::setTitle($winTitle);
    parent::setContent($winContent);
    parent::setParams($params);
  }

  public function getTableList($pagename)
  {
    return parent::getModuleTableList(array("ctitle" => _("Create a new Validation"),
					    "titles" => array( "contact_id", "project_id" ),
					    "cols" => array( "contact_id", "project_id" ) ), $pagename);
  }


}

?>