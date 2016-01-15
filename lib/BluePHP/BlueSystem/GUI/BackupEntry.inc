<?php

include_once("BluePHP/BlueSystem/Tools/BackupRestore.inc");
include_once("BluePHP/GUI/ComponentTemplate.inc");
include_once("BluePHP/GUI/MenuEntryState.inc");

/**
 */
class BackupEntry extends ComponentTemplate implements MenuEntryState
{
  private $m = null;

  public function __construct()
  {
    parent::__construct("BackupEntry", "BluePHP/BlueSystem/GUI/templates/");
    $this->m = new BackupRestore();
  }

  public function isEnabled()
  {
    return $this->m->haveSomethingToDo();
  }

}

?>
