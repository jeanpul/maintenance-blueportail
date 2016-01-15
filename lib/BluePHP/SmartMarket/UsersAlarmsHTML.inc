<?php

require_once("BluePHP/Utils/ModuleInterface.inc");
require_once("UsersAlarms.inc");
require_once("Users.inc");
require_once("Alarms.inc");
require_once("Sites.inc");

class UsersAlarmsHTML extends ModuleInterface
{
  function UsersAlarmsHTML($params)
  {
    parent::setName(_("UsersAlarms"));
    parent::setModule(new UsersAlarms());
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
	$winTitle = _("Links configuration");
	$winContent = "UsersAlarms"; 
      }
    else
      {    
	$winTitle = _("Links configuration");
	$winContent = "UsersAlarmsEdit"; 
      }
    parent::setTitle($winTitle);
    parent::setContent($winContent);
    parent::setParams($params);
  }

  protected function getTableInput($params)
  {
    $str = "<table>\n";    

    $module = new Users();
    $str .= get2CellsRow(_("User"), getSelectTemplate($module->getEntries(),
							 array( "value" => "name",
								"name" => "user_id",
								"key" => "id",
								"default" => $params["user_id"],
								"extra" => false )));

    $module = new Alarms();
    $str .= get2CellsRow(_("Alarm"), getSelectTemplate($module->getEntries(),
							 array( "value" => "name",
								"name" => "alarm_id",
								"key" => "id",
								"default" => $params["alarm_id"],
								"extra" => false )));

    $module = new Sites();
    $str .= get2CellsRow(_("Site"), getSelectTemplate($module->getEntries(),
							 array( "value" => "name",
								"name" => "site_id",
								"key" => "id",
								"default" => $params["site_id"],
								"extra" => false )));

    $str .= getInput(array( "value" => $params["sendEmail"] ? "1" : "0",
			    "type" => "hidden",
			    "id" => "sendEmail",
			    "name" => "sendEmail" ));

    $str .= getInput(array( "value" => $params["sendSMS"] ? "1" : "0",
			    "type" => "hidden",
			    "id" => "sendSMS",
			    "name" => "sendSMS" ));

    $extra = "onClick=\"ChangeValueOnCheck('checkSendEmail', 'sendEmail', '1', '0');\"";
    $extra .= $params["sendEmail"] ? " checked=\"checked\"" : "";
    $str .= get2CellsRow(_("Send E-mail"), getInput(array( "value" => "",
							   "type" => "checkbox",
							   "id" => "checkSendEmail",
							   "readonly" => false,
							   "extra" => $extra )));

    $extra = "onClick=\"ChangeValueOnCheck('checkSendSMS', 'sendSMS', '1', '0');\"";
    $extra .= $params["sendSMS"] ? " checked=\"checked\"" : "";
    $str .= get2CellsRow(_("Send SMS"), getInput(array( "value" => "",
							"type" => "checkbox",
							"id" => "checkSendSMS",
							"readonly" => false,
							"extra" => $extra )));    
    $str .= "</table>\n";
    return $str;
  }

  function getTableList($pagename)
  {
    return parent::getModuleTableList(array("ctitle" => _("Set an alarm for a user"),
					    "titles" => array( _("Name"), _("Alarm"), _("Site") ),
					    "cols" => array( "uname", "aname", "sname" ) ),
				      $pagename);
  }
  
}


?>
