<?php

require_once("BluePHP/Utils/ModuleInterface.inc");
require_once("BluePHP/Utils/Input.inc");
require_once("configs/WaitingTime.inc");

class WaitingTimeHTML extends ModuleInterface
{
  function WaitingTimeHTML($params)
  {
    parent::setName(_("WaitingTime"));
    parent::setModule(new WaitingTime());
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
	$winTitle = _("WaitingTime configuration");
	$winContent = "WaitingTime"; 
      }
    else
      {    
	$winTitle = _("WaitingTime configuration");
	$winContent = "WaitingTimeEdit"; 
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

    $str .= createTableRow(array( _("BTopLocalServer IP"),
				  createInputText($params["server_ip"],
						  array( "name" => "server_ip",
							 "readonly" => false,
							 "extra" => false ))
				  )
			   );

    $str .= createTableRow(array( _("WaitingTimeProcessing"),
				  createInputText($params["WaitingTimeProcessing"],
						  array( "name" => "WaitingTimeProcessing",
							 "readonly" => false,
							 "extra" => false ))
				  )
			   );

    $str .= createTableRow(array( _("Theme"),
				  createSelect( array( "Default" ),
						array( "key" => false,
						       "name" => "theme",
						       "value" => false,
						       "default" => $params["theme"],
						       "extra" => false ))
				  )
			   );

    $str .= createTableRow(array( _("Message"),
				  createInputTextArea($params["msg"],
						      array( "name" => "msg",
							     "rows" => 10,
							     "cols" => 40,
							     "extra" => false ))));

    $str .= "</table>\n";
    return $str;
  }

  function getTableList($pagename)
  {
    return parent::getModuleTableList(array("ctitle" => _("Create a new Waiting Time display"), 
					    "titles" => array( _("Id"), _("Server"), _("Processing"), _("Theme"), _("Message") ),
					    "cols" => array( "id", "server_ip", "WaitingTimeProcessing", "theme", "msg" ) ),
				      $pagename);
  }
  
}


?>