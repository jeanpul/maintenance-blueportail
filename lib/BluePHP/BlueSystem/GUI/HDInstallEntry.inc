<?php

include_once("BluePHP/BlueSystem/BlueSystem.inc");
include_once("BluePHP/GUI/ComponentTemplate.inc");
include_once("BluePHP/GUI/MenuEntryState.inc");

/**
 * Dynamic menu entry for
 * hd installation
 */
class HDInstallEntry extends ComponentTemplate implements MenuEntryState
{
  private $bs = null;

  public function __construct()
  {
    parent::__construct("HDInstallEntry", "BluePHP/BlueSystem/GUI/templates/");
    $this->bs = new BlueSystem();
  }

  public function isEnabled()
  {
    return $this->bs->isLive();
  }
}

?>

