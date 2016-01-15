<?php

include_once("BluePHP/BlueSystem/Tools/License.inc");
include_once("BluePHP/GUI/ComponentTemplate.inc");
include_once("BluePHP/GUI/MenuEntryState.inc");

/**
 * Dynamic menu entry for
 * license parameters
 */
class LicenseEntry extends ComponentTemplate implements MenuEntryState
{
  private $license = null;

  public function __construct()
  {
    parent::__construct("LicenseEntry", "BluePHP/BlueSystem/GUI/templates/");
    $this->license = new License();
  }

  public function isEnabled()
  {
    return $this->license->haveLicenseTools();
  }
}

?>

