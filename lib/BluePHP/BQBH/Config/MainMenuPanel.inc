<?php

require_once("BluePHP/BQBH/Config/BQueueFrame.inc");

/**
 * \class MainMenuPanel
 * Render the main menu from
 * the available entries. 
 */
class MainMenuPanel extends BQueueFrame
{
  public function __construct($id, $params)
  {
    parent::__construct($id, $params);
  }
}

?>