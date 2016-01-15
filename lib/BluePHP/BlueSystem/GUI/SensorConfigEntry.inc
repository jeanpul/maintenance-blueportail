<?php

include_once("BluePHP/BlueSystem/Tools/SensorConfig.inc");
include_once("BluePHP/GUI/ComponentTemplate.inc");
include_once("BluePHP/GUI/MenuEntryState.inc");

/**
 */
class SensorConfigEntry extends ComponentTemplate implements MenuEntryState
{
  private $m = null;

  public function __construct()
  {
    parent::__construct("SensorConfigEntry", "BluePHP/BlueSystem/GUI/templates/");
    $this->m = new SensorConfig();
  }

  public function isEnabled()
  {
    return $this->m->haveAllStatusOK();
  }

}

?>
