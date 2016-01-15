<?php

include_once("BluePHP/Utils/ModuleInterface.inc");
include_once("BluePHP/GUI/Window.inc");
include_once("BluePHP/Utils/Input.inc");
include_once("Status.inc");

class StatusHTML extends ModuleInterface
{
  function StatusHTML($params)
  {
    parent::setName(_("Status"));
    parent::setModule(new Status());
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

  function getTableList()
  {
    $str = "<table border='1'>\n";
    $str .= createTableHeaderRow(array( _("Id"), _("Logic"), _("Date"), _("Res") ));    
    $data = $this->getModule()->getEntries();
    foreach($data as $k => $v)
      {
	$str .= createTableRow(array( $v["id"], $v["logic_id"], $v["date"], $v["res"] ));
      }
    $str .= "</table>\n";
    return $str;
  }
}

