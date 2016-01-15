<?php

include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/GUI/MenuTable.inc");
include_once("BluePHP/GUI/MenuBarEntry.inc");
include_once("BluePHP/GUI/MenuEntryStatic.inc");
include_once("BluePHP/GUI/MenuEntryLink.inc");
include_once("BluePHP/GUI/MenuEntryEmpty.inc");
include_once("BluePHP/BTopLocalServer/Config.inc");
include_once("BluePHP/BTopLocalServer/BlueCountLang.inc");
include_once("BluePHP/BTopLocalServer/BCMFrame.inc");

class EntitiesMenu extends BCMFrame {
  
  public function __construct($id, $params)
  {
    parent::__construct($id, $params);
  }
}

?>