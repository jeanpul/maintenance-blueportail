<?php

include_once("BluePHP/GUI/BluePHPFrame.inc");
include_once("BluePHP/JQUERY/jquery.inc");

/**
 * \class TreeView
 * It's a container which use the jquery.treeview plugin
 * to provides a tree like view of a set of elements
 * \sa http://docs.jquery.com/Plugins/Treeview
 */
class TreeView extends BluePHPFrame 
{
  private $elts = array();

  /**
   * Create the TreeView object, add the jquery.treeview
   * to the library header and create the treeview during
   * the jquery ready function call.
   * \param $id the id used to create the treeview
   * \param $root the Application used to declare the library and define the tree
   */
  public function __construct($id, $root)
  {
    parent::__construct($id, "TreeView", $root);
    $this->getRoot()->addLib("treeview/jquery.treeview", array( "version" => null ));
    $this->getRoot()->pushReadyJS(jquery_id_call($this->getId() . " #tree", "treeview", array( "collapsed" => true,
											      "control" => "#sidetreecontrol",
											      "persist" => "location" )));
  }

  /**
   * Define the treeview elements
   */
  public function setElts($elts)
  {
    $this->elts = $elts;
  }

  /**
   * Retrieve the treeview elements
   */
  public function getElts()
  {
    return $this->elts;
  }
}

?>