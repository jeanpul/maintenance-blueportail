<?php

include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/BTopLocalServer/NumberingProcessing.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/OccupancyHTML.inc");

/**
 * \class OccupancyGaugeHTML
 */
class OccupancyGaugeHTML extends OccupancyHTML
{
  public function __construct($id, $params)
  {
    parent::__construct($id, $params);
  }
}

?>