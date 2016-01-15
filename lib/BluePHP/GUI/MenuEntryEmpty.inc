<?php

include_once("BluePHP/GUI/MenuEntryStatic.inc");

/**
 * \class MenuEntryEmpty
 * Provides an empty entry for the menu. If 
 * you want a separation between entries or
 * add a empty cell in a icon table
 */
class MenuEntryEmpty extends MenuEntryStatic
{
  /**
   * Simply create a span element with a dummy id
   */
  public function __construct()
  {
    parent::__construct("<span id=\"dummy\"></span>");
  }

}

?>