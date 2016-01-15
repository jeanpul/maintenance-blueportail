<?php

include_once("BluePHP/System/System.inc");
include_once("BluePHP/BlueSystem/GUI/BSWindow.inc");

class DateWindow extends BSWindow
{
  private $system = null;

  public function __construct($root)
  {
    parent::__construct(array( "main" => _("BlueEyeVideo system - Time & Date"),
			       "sub" => _("Time & Date") ),
			"Logs", 
			$root,
			"BluePHP/BlueSystem/GUI/templates/");
    $this->system = new System();
  }

}

?>
