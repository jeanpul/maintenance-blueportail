<?php

include_once("BluePHP/GUI/BluePHPFrame.inc");
include_once("BluePHP/GUI/Container.inc");
include_once("BluePHP/JQUERY/jquery.inc");

/**
 * \class MenuBar
 * It's a container which use the BluePHP jquery plugin menubar
 * to provides a dynamic menu which automatically handle
 * the selected element by using an active class. It also
 * provides onclick callback for the menu elements.
 */
class MenuBar extends BluePHPFrame implements Container
{
  /** Menu elements set */
  private $elts = array();

  /**
   * Create the MenuBar object, declare the jquery.menubar library and
   * define the menubar element into the jquery onReady function
   * \param $id the id that contains the menubar elements
   * \param $root the Application used to add the library and the definitions
   * \param $cb optionale callback to be used when the user click on a menu entry
   */
  public function __construct($id, $root, $cb = null)
  {
    parent::__construct($id, "MenuBar", $root);
    $this->getRoot()->addLib("jquery.menubar", array("path" => "/BluePHP/JQUERY/",
						     "version" => null ));
    $params = null; $paramsReplace = null;
    if(!is_null($cb))
      {
	$params = array( "clickCb" => "%funccb%" );
	$paramsReplace = array( "%funccb%" => $cb );
      }
    $this->getRoot()->pushReadyJS(jquery_id_call($this->getId(), "menubar", $params, $paramsReplace));
  }

  /**
   * Add a new menu entry (renderable)
   */
  public function add($elt)
  {
    $this->elts[] = $elt;
  }

  /**
   * Retrieve all the menu entries
   */
  public function getElts()
  {
    return $this->elts;
  }
}


?>
