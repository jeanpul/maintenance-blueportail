<?php

include_once("BluePHP/GUI/BluePHPFrame.inc");

/**
 * \class MenuTable
 * Create a menu using table rows and li for each
 * table column. The class linkList is used to 
 * render the element properly. To add a new row,
 * simply put an element to null. The table have
 * a Menu class and its surrounded by a div with
 * a MenuTable class
 */
class MenuTable extends BluePHPFrame
{
  private $entries = array();

  /**
   * Create the menu table with the specified renderable entries.
   * \param $id html attribute id
   * \param $entries the set of component (with null element to separate rows)
   * \param $root Application reference
   */
  public function __construct($id, $entries, $root)
  {
    parent::__construct($id, "MenuTable", $root);
    $this->entries = $entries;
  }

  /**
   * Returns the menu entries
   */ 
  public function getEntries()
  {
    return $this->entries;
  }
}

?>