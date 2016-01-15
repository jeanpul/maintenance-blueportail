<?php

include_once("BluePHP/GUI/BluePHPFrame.inc");

/**
 * \class Label
 * \warning should not use this class
 * \todo check if this is used or not
 */
class Label extends BluePHPFrame
{
  private $text = array();
  
  /**
   * Construct
   * \param $id html id
   * \param $root Application
   * \param $text string to display
   */
  public function __construct($id, $root, $text)
  {
    parent::__construct($id, "Label", $root);
    $this->text = $text;
  }

  /**
   * Returns text message
   */
  public function getText()    
  {
    return $this->text;
  }
}

?>