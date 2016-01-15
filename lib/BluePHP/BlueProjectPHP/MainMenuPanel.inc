<?php

require_once("BluePHP/BlueProjectPHP/BPFrame.inc");

/**
 * \class MainMenuPanel
 * Render the main menu from
 * the available entries. 
 */
class MainMenuPanel extends BPFrame
{
  /**
   *  Parameters could pass the active menu entry.
   */
  public function __construct($id, $params)
  {
    parent::__construct($id, $params);
  }
}

?>