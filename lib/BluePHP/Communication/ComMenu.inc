<?php

include_once("BluePHP/GUI/MenuTable.inc");
include_once("BluePHP/GUI/MenuBarEntry.inc");
include_once("BluePHP/Communication/ComFrame.inc");

/**
 * \class ComMenu
 */
class ComMenu extends ComFrame {

  public function __construct($id, $params)
  {
    parent::__construct($id, $params);
  }
}

?>