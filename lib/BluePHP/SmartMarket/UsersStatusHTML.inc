<?php

require_once("BluePHP/Utils/ModuleInterface.inc");
require_once("BluePHP/GUI/Window.inc");
require_once("UsersStatus.inc");

class UsersStatusHTML extends ModuleInterface
{
  function UsersStatusHTML($params)
  {
    parent::setName(_("UsersStatus"));
    parent::setModule(new UsersStatus());
    $this->setWindow($params);
  }

  function setWindow($params)
  {
    $winTitle = _("Users Status List");
    $winContent = "UsersStatus"; 
    parent::setTitle($winTitle);
    parent::setContent($winContent);
    parent::setParams($params);
  }

  function getTableList($pagename)
  {
    $str = "<table border='1'>\n";
    $str .= "<tr><th>User</th><th>Alarm</th><th>Site</th><th>Entity</th>" . 
      "<th>Last mail</th><th>Last SMS</th><th>Nb Mail</th><th>Nb SMS</th></tr>\n";
    foreach($this->getModule()->getEntries() as $k => $v)
      {
	$str .= "<tr><td>" . $v["uname"] . "</td><td>" . $v["aname"] . 
	  "</td><td>" . $v["sname"] . "</td><td>" .
	  $v["site_entity_id"] . "</td><td>" . $v["last_mail_raised_date"] . "</td><td>" .	  
	  $v["last_sms_raised_date"] . "</td><td>" . $v["nb_mail_raised"] . "</td><td>" . $v["nb_sms_raised"] . "</td></tr>\n";
      }    
    $str .= "</table>\n";
    return $str;
  }

}

