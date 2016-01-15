<?php

require_once("BluePHP/GUI/Frame.inc");

/**
 * \class DashMainMenu
 * Render the main menu from
 * the available entries. 
 */
class DashMainMenu extends Frame
{
  private $params = null;

  /**
   *  Parameters could pass the active menu entry.
   */
  public function __construct($id, $params)
  {
    parent::__construct($id, get_class($this), "BluePHP/Dashboards/templates");
    $this->params = $params;
  }

  protected function getParams()
  {
    return $this->params;
  }
}

?>