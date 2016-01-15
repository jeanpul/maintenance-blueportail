<?php

require_once("BluePHP/Utils/ModuleInterface.inc");
require_once("BluePHP/Utils/Input.inc");
require_once("logics/DualDisplay.inc");

class DualDisplayHTML extends ModuleInterface
{
  function DualDisplayHTML($params)
  {
    parent::setName(_("DualDisplay"));
    parent::setModule(new DualDisplay());
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
	$winTitle = _("DualDisplay configuration");
	$winContent = "DualDisplay"; 
      }
    else
      {    
	$winTitle = _("DualDisplay configuration");
	$winContent = "DualDisplayEdit"; 
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

    $str .= createTableRow(array( _("Config 1"),
				  createInputText($params["config1"],
						  array( "name" => "config1",
							 "readonly" => false,
							 "extra" => false ))
				  )
			   );

    $str .= createTableRow(array( _("Config 1 duration (seconds)"),
				  createInputText($params["config1_time"],
						  array( "name" => "config1_time",
							 "readonly" => false,
							 "extra" => false ))
				  )
			   );

    $str .= createTableRow(array( _("Config 2"),
				  createInputText($params["config2"],
						  array( "name" => "config2",
							 "readonly" => false,
							 "extra" => false ))
				  )
			   );

    $str .= createTableRow(array( _("Config 2 duration (seconds)"),
				  createInputText($params["config2_time"],
						  array( "name" => "config2_time",
							 "readonly" => false,
							 "extra" => false ))
				  )
			   );

    $str .= "</table>\n";
    return $str;
  }

  function getTableList($pagename)
  {
    return parent::getModuleTableList(array("ctitle" => _("Create a new Dual Display"), 
					    "titles" => array( _("Id"), _("Config1"), _("Time1"), _("Config2"), _("Time2") ),
					    "cols" => array( "id", "config1", "config1_time", "config2", "config2_time" ) ),
				      $pagename);
  }
  
}


?>