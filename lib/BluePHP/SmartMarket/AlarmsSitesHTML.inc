<?php

require_once("BluePHP/Utils/ModuleInterface.inc");
require_once("AlarmsSites.inc");
require_once("Alarms.inc");
require_once("Sites.inc");

class AlarmsSitesHTML extends ModuleInterface
{
  function AlarmsSitesHTML($params)
  {
    parent::setName(_("AlarmsSites"));
    parent::setModule(new AlarmsSites());
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
	$winTitle = _("Alarms Sites Links configuration");
	$winContent = "AlarmsSites"; 
      }
    else
      {    
	$winTitle = _("Alarms Sites Links configuration");
	$winContent = "AlarmsSitesEdit"; 
      }
    parent::setTitle($winTitle);
    parent::setContent($winContent);
    parent::setParams($params);
  }

  protected function getTableInput($params)
  {
    $str = "<table>\n";    

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

    $str .= get2CellsRow(_("Comment"), getInputTextArea($params["comment"],
							array( "name" => "comment",
							       "rows" => 4,
							       "cols" => 40,
							       "extra" => false )));

    $str .= "</table>\n";
    return $str;
  }

  function getTableList($pagename)
  {
    return parent::getModuleTableList(array("ctitle" => _("Set an alarm for a site"),
					    "titles" => array( _("Alarm"), _("Site"), _("Comment") ),
					    "cols" => array( "aname", "sname", "comment" ) ),
				      $pagename);
  }
  
}


?>
