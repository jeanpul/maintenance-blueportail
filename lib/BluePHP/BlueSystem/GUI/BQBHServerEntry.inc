<?php

include_once("BluePHP/BQBH/BQBHServerLicense.inc");
include_once("BluePHP/GUI/ComponentTemplate.inc");
include_once("BluePHP/GUI/MenuEntryState.inc");

/**
 */
class BQBHServerEntry extends ComponentTemplate implements MenuEntryState
{
  private $m = null;

  public function __construct()
  {
    parent::__construct("BQBHServerEntry", "BluePHP/BlueSystem/GUI/templates/");
    $this->m = new BQBHServerLicense();
  }

  public function isEnabled()
  {
    return $this->m->nbServerMax() > 0;
  }

}

?>
