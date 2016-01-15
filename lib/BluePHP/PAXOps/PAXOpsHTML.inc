<?php

include_once("BluePHP/GUI/Window.inc");
include_once("BluePHP/Utils/ModuleInterface.inc");
include_once("BluePHP/Utils/Input.inc");
include_once("PAXOps.inc");

class PAXOpsHTML extends ModuleInterface
{
  function PAXOpsHTML($params)
  {
    parent::setName(_("PAXOps"));
    parent::setModule(new PAXOps());
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
	$winTitle = _("PAXOps configuration");
	$winContent = "PAXOps"; 
      }
    else
      {    
	$winTitle = _("PAXOps configuration");
	$winContent = "PAXOpsEdit"; 
      }
    parent::setTitle($winTitle);
    parent::setContent($winContent);
    parent::setParams($params);
  }

  protected function getTableInput($params)
  {
    $str = "<table>\n";    
    $str .= createTableRow( array( _("ClientId"), createInputText($params["clientId"],
								  array( "name" => "clientId",
									 "readonly" => false,
									 "extra" => false )))
			    );
    $str .= createTableRow( array( _("Sensor Id"), createInputText($params["sensorId"],
								   array( "name" => "sensorId",
									  "readonly" => false,
									  "extra" => false )))
			    );

    $str .= createTableRow( array( _("bluehttp_server"), createInputText($params["bluehttp_server"],
									 array( "name" => "bluehttp_server",
										"readonly" => false,
										"extra" => false,
										"OnNullValue" => "localhost" )))
			    );

    $str .= createTableRow( array( _("bluehttp_port"), createInputText($params["bluehttp_port"],
								       array( "name" => "bluehttp_port",
									      "readonly" => false,
									      "extra" => false,
									      "OnNullValue" => "80" )))
			    );

    $str .= createTableRow( array( _("bluehttp_script"), createInputText($params["bluehttp_script"],
									 array( "name" => "bluehttp_script",
										"readonly" => false,
										"extra" => false,
										"OnNullValue" => "/BTopLocalServer/GrabCounter.php" )))
			    );

    $str .= createTableRow( array( _("Comment"), createInputTextArea($params["comment"],
								     array( "name" => "comment",
									    "rows" => 4,
									    "cols" => 40,
									    "extra" => false )))
			    );
    $str .= "</table>\n";
    return $str;
  }

  function getTableList($pagename)
  {
    return parent::getModuleTableList(array("ctitle" => _("Add a new PAXOps"),
					    "titles" => array( _("ClientId"), _("SensorId"), _("BlueHTTP") ),
					    "cols" => array( "clientId", "sensorId", "bluehttp_server" ) ),
				      $pagename);
  }
  
}


?>
