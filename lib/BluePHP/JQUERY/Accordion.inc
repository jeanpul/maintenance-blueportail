<?php

include_once("BluePHP/GUI/BluePHPFrame.inc");
include_once("BluePHP/GUI/Container.inc");
include_once("BluePHP/JQUERY/jquery.inc");

/**
 * \class Accordion
 * It's a container which use the jquery.accordion plugin
 * to provides a set of renderable elements that could be minimized and
 * expanded.
 * \sa http://docs.jquery.com/UI/Accordion
 */
class Accordion extends BluePHPFrame implements Container
{
  /**
   * Elements set
   */
  private $elts = array();

  /**
   * Create the Accordion object and add the definition
   * of the accordion into the onReady jquery function.
   * \param $id id used to create the accordion
   * \param $root Application used to create the definition
   */
  public function __construct($id, $root)
  {
    parent::__construct($id, "Accordion", $root);
    $this->getRoot()->pushReadyJS(jquery_id_call($this->getId(), "accordion"));
  }

  /**
   * Add a new renderable element
   */
  public function add($elt)
  {
    $this->elts[] = $elt;
    return true;
  }

  /**
   * Returns the elements set
   */
  public function getElts()    
  {
    return $this->elts;
  }
}

?>