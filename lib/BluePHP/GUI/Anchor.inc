<?php

include_once("BluePHP/GUI/Component.inc");

/**
 * \class Anchor
 * html href encapsulation renderable
 */
class Anchor implements Component
{
  private $anchor = null;

  /**
   * Build the anchor string
   * \param $pagename url file location for the href
   * \param $label text displayed for the a tag
   * \param $params array of pairs value that will be concatened to the $pagename parameter
   * \sa http_build_query
   */
  public function __construct($pagename, $label, $params = array())
  {
    $this->anchor = "<a href=\"$pagename" . "?" . http_build_query($params) . "\">$label</a>";
  }
  
  /**
   * Simply display the html tag
   */
  public function render()
  {
    echo $this->anchor;
  }
}

?>