<?php

require_once("BluePHP/Utils/ModuleInterface.inc");
require_once("BluePHP/GUI/Window.inc");
require_once("AlarmsStatus.inc");

class AlarmsStatusHTML extends ModuleInterface
{
  function AlarmsStatusHTML($params)
  {
    parent::setName(_("AlarmsStatus"));
    parent::setModule(new AlarmsStatus());
    $this->setWindow($params);
  }

  function setWindow($params)
  {
    $winTitle = _("Alarms Status List");
    $winContent = "AlarmsStatus"; 
    parent::setTitle($winTitle);
    parent::setContent($winContent);
    parent::setParams($params);
  }

  function getTableList($pagename)
  {
    $str = "<table border='1'>\n";
    $str .= "<tr><th>Alarm</th><th>Site</th><th>Entity</th><th>ttr</th>" . 
      "<th>Last ttr</th><th>Last raised</th><th>Nb raised</th></tr>\n";    
    foreach($this->getModule()->getEntries() as $k => $v)
      {
	$str .= "<tr><td>" . $v["aname"] . "</td><td>" . $v["sname"] . "</td><td>" .
	  $v["site_entity_id"] . "</td><td>" . $v["ttr"] . "</td><td>" .
	  $v["last_ttr"] . "</td><td>" . $v["last_raised"] . "</td><td>" . $v["nb_raised"] . "</td></tr>\n";
      }    
    $str .= "</table>\n";
    return $str;
  }

}

