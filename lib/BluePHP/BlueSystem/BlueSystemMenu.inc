<?php

include_once("BluePHP/System/Network.inc");
include_once("BluePHP/GUI/MenuTable.inc");
include_once("BluePHP/GUI/MenuBarEntry.inc");
include_once("BluePHP/GUI/MenuEntryStatic.inc");
include_once("BluePHP/GUI/MenuEntryEmpty.inc");
include_once("BluePHP/GUI/MenuEntryButton.inc");
include_once("BluePHP/BlueSystem/GUI/BSFrame.inc");
include_once("BluePHP/BlueSystem/Tools/BackupRestore.inc");
include_once("BluePHP/BlueSystem/BlueSystem.inc");

/**
 * \class BlueSystemMenu
 * displays the BlueSystem main menu
 * \addtogroup BlueSystem
 * @{
 */
class BlueSystemMenu extends BSFrame {

  /**
   * Set the module to be BlueSystem. If you want to
   * add a page to the main menu simply inherit from this
   * and add the main menu as its immediate parent.
   * \param $id html id
   * \param $params frame parameters
   */
  public function __construct($id, $params)
  {
    parent::__construct($id, $params);
    parent::setSystem("BlueSystem");
  }
}

/** @} */

?>