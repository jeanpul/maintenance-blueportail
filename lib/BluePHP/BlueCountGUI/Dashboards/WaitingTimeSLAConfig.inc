<?php

include_once("BluePHP/BlueCountGUI/Config.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/OccupancyConfig.inc");

class WaitingTimeSLAConfig extends OccupancyConfig
{
  function __construct($sharedCon = null)
  {
    parent::__construct($sharedCon);
  }
}

?>
