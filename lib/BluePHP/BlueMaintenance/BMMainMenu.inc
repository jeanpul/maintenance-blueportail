<?php

require_once("BluePHP/GUI/Frame.inc");

/**
 * \class BMMainMenu
 * Render the main menu from
 * the available entries. 
 */
class BMMainMenu extends Frame
{
  private $params = null;

  /**
   *  Parameters could pass the active menu entry.
   */
  public function __construct($id, $params)
  {
    parent::__construct($id, get_class($this), "BluePHP/BlueMaintenance/templates");
    $this->params = $params;
  }

  protected function getParams()
  {
    return $this->params;
  }
}

?>