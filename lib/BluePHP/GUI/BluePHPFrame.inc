<?php

include_once("BluePHP/GUI/Frame.inc");

/**
 * \class BluePHPFrame
 * Simply extends Frame by using a member to store
 * a reference to the Application element in order
 * to provide the component to call the Application
 * method before and during the rendering. For example
 * to retrieve the page arguments, to add some css and 
 * javascript includes. Moreover set the template
 * basepath to be into the BluePHP/GUI/templates directory
 */
class BluePHPFrame extends Frame
{
  private $root = null;

  /**
   * Create the element.
   * \param $id to identify the html part
   * \param $content basename of the template
   * \param $root reference to the Application
   * \sa Frame ComponentTemplate
   */
  public function __construct($id, $content, $root)
  {
    parent::__construct($id, $content, "BluePHP/GUI/templates");
    $this->root = $root;
  }

  /**
   * Returns the Application reference
   */
  protected function getRoot()
  {
    return $this->root;
  }
}

?>