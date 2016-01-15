<?php

include_once("BluePHP/GUI/BluePHPFrame.inc");

/**
 * \class Dialog
 * Represents a minimum structure to display
 * messages. A dialog is composed of a main
 * div and a sub div which is called the Content
 * and identified by DialogIdContent
 */
class Dialog extends BluePHPFrame
{
  private $title = null;
  private $content = "";
  private $header = "";

  /**
   * The dialog have its title
   * \param $id html identification
   * \param $title dialog title
   * \param $root Application reference
   */
  public function __construct($id, $title, $root)
  {
    parent::__construct($id, "Dialog", $root);
    $this->title = $title;
  }

  /**
   * Returns the dialog title
   */
  public function getTitle()
  {
    return $this->title;
  }
  
  public function setContent($c)
  {
    $this->content = $c;
  }

  public function getContent()
  {
    return $this->content;
  }

  public function setHeader($h)
  {
    $this->header = $h;
  }

  public function getHeader()
  {
    return $this->header;
  }

}

?>