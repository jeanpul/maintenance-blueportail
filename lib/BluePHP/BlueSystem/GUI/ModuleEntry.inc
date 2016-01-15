<?php

include_once("BluePHP/BlueSystem/Tools/Modules.inc");
include_once("BluePHP/GUI/ComponentTemplate.inc");
include_once("BluePHP/GUI/MenuEntryState.inc");

/**
 */
class ModuleEntry extends ComponentTemplate implements MenuEntryState
{
  private $m = null;

  public function __construct()
  {
    parent::__construct("ModuleEntry", "BluePHP/BlueSystem/GUI/templates/");
    $this->m = new Modules();
  }

  public function isEnabled()
  {
    return $this->m->haveModules();
  }
}

?>
