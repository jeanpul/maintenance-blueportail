<?php

include_once("BluePHP/GUI/BluePHPFrame.inc");
include_once("BluePHP/GUI/Container.inc");

/**
 * \class Concat
 * Simply call the render method for a set of element
 * \sa VBox
 */
class Concat extends BluePHPFrame implements Container
{
  private $elts = array();
  
  /**
   * \param $elts the set of element
   */
  public function __construct($elts = array())
  {
    parent::__construct(null, "Concat", null);
    $this->elts = $elts;
  }

  /**
   * Push a new element at the end of the set
   */
  public function add($elt)
  {
    $this->elts[] = $elt;
  }

  /**
   * Returns the set of element
   */
  public function getElts()
  {
    return $this->elts;
  }
}

?>