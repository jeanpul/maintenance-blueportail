<?php

require_once("BluePHP/Utils/ModuleInterface.inc");
require_once("BluePHP/GUI/Window.inc");
require_once("BluePHP/Utils/Input.inc");
require_once("Status.inc");

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
    $str .= createTableHeaderRow(array( _("Id"), _("Src"), _("Dst"), _("Date"), _("Content"), _("Res") ));    
    $data = $this->getModule()->getEntries();
    foreach($data as $k => $v)
      {
	$str .= createTableRow(array( $v["id"], $v["src"], $v["dst"], $v["date"], $v["content"], $v["res"] ));
      }
    $str .= "</table>\n";
    return $str;
  }
}

