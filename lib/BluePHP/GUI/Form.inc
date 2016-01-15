<?php

include_once("BluePHP/GUI/BluePHPFrame.inc");

/**
 * \class Form
 * html form abstraction
 * \warning should no be used
 * \todo should check if this is used or not
 */
class Form extends BluePHPFrame
{
  private $elts = array();
  private $visible = true;

  /**
   * \param $id html id attribute
   * \param $root Application reference
   * \param $params form parameters
   * \param $paramsOpt form optional parameters
   */
  public function __construct($id, $root, $params, $paramsOpt = null)
  {
    parent::__construct($id, "Form", $root);    
    $this->elts = $params;
  }

  /**
   * Retrieve the form parameters
   */
  public function getElts()
  {
    return $this->elts;
  }

  public function setVisible($t)
  {
    $this->visible = $t;
  }

  public function isVisible()
  {
    return $this->visible;
  }
}

?>