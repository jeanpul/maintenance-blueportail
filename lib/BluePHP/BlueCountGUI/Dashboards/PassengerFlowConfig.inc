<?php

include_once("BluePHP/BlueCountGUI/Config.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/FlowHistoryConfig.inc");

class PassengerFlowConfig extends FlowHistoryConfig
{
  function __construct($sharedCon = null)
  {
    parent::__construct($sharedCon);
  }
}

?>