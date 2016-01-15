<?php

include_once("BluePHP/System/System.inc");
include_once("BluePHP/BlueSystem/GUI/BSWindow.inc");

class RebootWindow extends BSWindow
{
  private $system = null;

  public function __construct($root)
  {
    parent::__construct(array( "main" => _("BlueEyeVideo system - Reboot system"),
			       "sub" => _("Reboot system") ),
			"Reboot", 
			$root,
			"BluePHP/BlueSystem/GUI/templates/");
    $this->system = new System();
  }

  protected function doReboot()
  {
    $this->system->reboot();
  }
}

?>