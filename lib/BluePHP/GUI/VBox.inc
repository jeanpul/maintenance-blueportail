<?php

include_once("BluePHP/GUI/BluePHPFrame.inc");
include_once("BluePHP/GUI/Container.inc");

/**
 * \class VBox
 * Container used to display a set of renderable one after
 * the other. This is called vertical box because of the
 * natural html flow is vertical. In fact this is only
 * the calls to the render method of each contained elements.
 * A div with a "VBox" class surround all the elements.
 */
class VBox extends BluePHPFrame implements Container
{
  private $elts = array();
  
  /**
   * Create the container box
   * \param $id html id
   * \param $root reference Application
   * \param $elts optional renderable elements list
   */
  public function __construct($id, $root, $elts = array())
  {
    parent::__construct($id, "VBox", $root);
    $this->elts = $elts;
  }

  /**
   * Push a new element at end of the list
   */
  public function add($elt)
  {
    $this->elts[] = $elt;
  }

  /**
   * Returns all the elements
   */
  public function getElts()
  {
    return $this->elts;
  }
}

?>