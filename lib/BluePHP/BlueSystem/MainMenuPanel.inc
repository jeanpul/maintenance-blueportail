<?php

require_once("BluePHP/GUI/Frame.inc");

/**
 * \class MainMenuPanel
 * Render the main menu from
 * the available entries. 
 */
class MainMenuPanel extends Frame
{
  private $params = null;

  /**
   *  Parameters could pass the active menu entry.
   */
  public function __construct($id, $params)
  {
    parent::__construct($id, "MainMenuPanel", "BluePHP/BlueSystem/GUI/templates");    
    $this->params = $params;
  }
}

?>