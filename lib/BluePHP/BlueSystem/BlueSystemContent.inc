<?php

require_once("BluePHP/BlueSystem/GUI/BSFrame.inc");

/**
 * \class BlueSystemContent
 * Render the content part of the application
 * for the BlueSystem that is the submenu
 * and the active window
 */
class BlueSystemContent extends BSFrame
{
  /**
   * Parameters should pass the active window class name
   * that will be instantiated
   */
  public function __construct($id, $params)
  {
    parent::__construct($id, $params);
  }  
}

?>