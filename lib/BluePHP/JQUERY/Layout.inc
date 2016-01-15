<?php

include_once("BluePHP/GUI/BluePHPFrame.inc");
include_once("BluePHP/GUI/Container.inc");
include_once("BluePHP/JQUERY/jquery.inc");

/**
 * \class Layout
 * It's a container which use the jquery.layout plugin
 * to provides a page composed of 5 distinct independant parts :
 * north, west, center, east and south. 
 * \sa http://layout.jquery-dev.net/documentation.html
 */
class Layout extends BluePHPFrame implements Container
{
  /**
   * store the elements at each possible position
   */
  private $elts = array( "north" => null,
			 "west" => null,
			 "center" => null,
			 "east" => null,
			 "south" => null );

  /**
   * Construction of the element ; add the jquery.layout library to the
   * Application and put the definition of the javascript layout regarding the id
   * into the jquery onReady function
   * \param $id the id attribute value used to create the layout
   * \param $root a reference to the Application used to add the library and to create the layout definition
   */
  public function __construct($id, $root)
  {
    parent::__construct($id, "Layout", $root);
    $this->getRoot()->addLib("jquery.layout");
    $this->getRoot()->pushReadyJS(jquery_id_call($this->getId(), "layout", array( "applyDefaultStyles" => "true" )));
  }

  /**
   * Add a renderable element to the next empty position. 
   * Layout positions order is : north, west, center, east and south
   * \param $elt a renderable element
   * \param $pos optional position values should be one of north, west, center, east, south
   */
  public function add($elt, $pos = FALSE)
  {
    if($pos == FALSE)
      {
	$pos = array_search(null, $this->elts); // look for a new cell
      }
    if($pos !== FALSE)
      {
	$this->elts[$pos] = $elt;
      }
    return $pos !== FALSE;
  }

  /**
   * Returns all the positions with their associated element.
   * A NULL value is used If no element is present at a specific position
   */
  public function getElts()
  {
    return $this->elts;
  }

  /**
   * Return the element at a specific position or NULL if no element is present
   * \param $pos should be one of north, west, center, east, south
   */
  public function getElt($pos)
  {
    return $this->elts[$pos];
  }
}

?>
