<?php

include_once("BluePHP/GUI/BluePHPFrame.inc");

/**
 * \class Button
 */ 
class Button extends BluePHPFrame
{
  private $attrs = array();

  public function __construct($id, $attrs)
  {
    parent::__construct($id, "Button", null);
    $this->attrs = $attrs;
  }

  public function getLabel()
  {
    return $this->attrs["label"];
  }

  public function eventClick($cb)
  {
    return '$("#' . $this->getId() . '").click(function() {' .
      $cb . '});';
  }
}

?>