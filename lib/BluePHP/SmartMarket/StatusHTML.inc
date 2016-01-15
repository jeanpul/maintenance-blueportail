<?php

require_once("BluePHP/Utils/ModuleInterface.inc");
require_once("BluePHP/GUI/Window.inc");
require_once("AlarmsStatusHTML.inc");
require_once("UsersStatusHTML.inc");

class StatusHTML extends ModuleInterface
{
  var $alarmsStatus = false;
  var $usersStatus = false;

  function StatusHTML($params)
  {
    parent::setName(_("Status"));
    parent::setModule(NULL);
    $this->alarmsStatus = new AlarmsStatusHTML($params);
    $this->usersStatus = new UsersStatusHTML($params);
    $this->setWindow($params);
  }

  function setWindow($params)
  {
    $winTitle = _("Status");
    $winContent = "Status"; 
    parent::setTitle($winTitle);
    parent::setContent($winContent);
    parent::setParams($params);
  }
}

