<?php

include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/Utils/js_lang.inc");
include_once("BluePHP/GUI/MenuTable.inc");
include_once("BluePHP/GUI/MenuBarEntry.inc");
include_once("BluePHP/GUI/MenuEntryLink.inc");
include_once("BluePHP/GUI/MenuEntryEmpty.inc");
include_once("BluePHP/BlueCountGUI/Config.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/DashboardsFrame.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/Dashboards.inc");

class DashboardMenu extends DashboardsFrame {

  public function __construct($id, $params)
  {
    parent::__construct($id, $params);
  }  
}

?>