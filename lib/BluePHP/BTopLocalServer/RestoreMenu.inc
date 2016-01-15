<?php

include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/GUI/MenuTable.inc");
include_once("BluePHP/GUI/MenuBarEntry.inc");
include_once("BluePHP/GUI/MenuEntryWindow.inc");
include_once("BluePHP/GUI/MenuEntryEmpty.inc");
include_once("BluePHP/BTopLocalServer/Config.inc");
include_once("BluePHP/BTopLocalServer/BCMFrame.inc");

class RestoreMenu extends BCMFrame 
{
  public function __construct($id, $params)
  {
    parent::__construct($id, $params);
  }
}

?>