<?php

include_once("BluePHP/GUI/BluePHPFrame.inc");

/**
 * \class DialogWarning
 * Provides minimum structure to display information 
 * message to the user. It simply use a div with class "warning"
 */
class DialogWarning extends BluePHPFrame
{
  private $titles = null;

  /**
   * Create the dialog
   * \param $id html attribute
   * \param $titles parameters array that should have a "title" and a "msg" values
   * \param $root Application reference
   */
  public function __construct($id, $titles, $root)
  {
    parent::__construct($id, "DialogWarning", $root);
    $this->titles = $titles;
  }

  /**
   * Returns the dialog title
   */
  public function getTitle()
  {
    return $this->titles["title"];
  }

  /**
   * Returns the message content
   */
  public function getMsg()
  {
    return $this->titles["msg"];
  }
}

?>