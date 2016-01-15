<?php

include_once("BluePHP/GUI/MenuTable.inc");
include_once("BluePHP/GUI/MenuBarEntry.inc");
include_once("BluePHP/GUI/MenuEntryStatic.inc");
include_once("BluePHP/GUI/MenuEntryEmpty.inc");
include_once("BluePHP/GUI/MenuEntryButton.inc");
include_once("BluePHP/BlueProjectPHP/BPFrame.inc");

/**
 */
class BlueProjectMenu extends BPFrame {

  /**
   */
  public function __construct($id, $params)
  {
    parent::__construct($id, $params);
  }
}

/** @} */

?>