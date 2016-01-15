<?php

include_once("BluePHP/System/System.inc");
include_once("BluePHP/BlueSystem/GUI/BSWindow.inc");

/**
 * \class LogsWindow
 * \deprecated not used anymore ?
 */
class LogsWindow extends BSWindow
{
  private $system = null;

  /**
   * \deprecated to be removed
   */
  public function __construct($root)
  {
    parent::__construct(array( "main" => _("BlueEyeVideo system - Log Viewer"),
			       "sub" => _("Log Viewer") ),
			"Logs", 
			$root,
			"BluePHP/BlueSystem/GUI/templates/");
    $this->system = new System();
  }

}

?>
