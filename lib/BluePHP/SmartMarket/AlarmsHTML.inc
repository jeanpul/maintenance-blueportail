<?php

require_once("BluePHP/Utils/ModuleInterface.inc");
require_once("BluePHP/GUI/Window.inc");
require_once("BluePHP/Utils/Input.inc");
require_once("Alarms.inc");

class AlarmsHTML extends ModuleInterface
{
  function AlarmsHTML($params)
  {
    parent::setName(_("Alarms"));
    parent::setModule(new Alarms());
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
	$winTitle = _("Alarms configuration");
	$winContent = "Alarms"; 
      }
    else
      {    
	$winTitle = _("Alarms configuration");
	$winContent = "AlarmsEdit"; 
      }
    parent::setTitle($winTitle);
    parent::setContent($winContent);
    parent::setParams($params);
  }

  protected function getTableInput($params)
  {
    $str = "<table>\n";    
    $str .= get2CellsRow(_("Name"), getInputText($params["name"],
					      array( "name" => "name",
						     "readonly" => false,
						     "extra" => false )));

    $module = new Alarms();
    $str .= get2CellsRow(_("Alarm type"), createSelect($module->getFunctions(),
						       array( "value" => "name",
							      "name" => "function_id",
							      "key" => "id",
							      "default" => $params["function_id"],
							      "extra" => false )));

    $str .= get2CellsRow(_("Time to raise"), createSelect(array( 1, 2, 5, 10, 15, 30 ), 
							  array( "value" => false,
								 "name" => "ttr",
								 "key" => false,
								 "default" => $params["ttr"],
								 "extra" => false )));
    
    $str .= get2CellsRow(_("E-mail message"), getInputTextArea($params["email"],
							    array( "name" => "email",
								   "rows" => 4,
								   "cols" => 40,
								   "extra" => false )));

    $str .= get2CellsRow(_("SMS message"), getInputTextArea($params["sms"],
							 array( "name" => "sms",
								"rows" => 4,
								"cols" => 40,
								"extra" => false )));    
    $str .= "</table>\n";
    return $str;
  }

  function getTableList($pagename)
  {
    return parent::getModuleTableList(array("ctitle" => _("Create an new Alarm"),
					    "titles" => array( _("Name"), _("Type"), _("Time to Raise") ),
					    "cols" => array( "name", "fname", "ttr" ) ),
				      $pagename);
  }
  
}


?>
