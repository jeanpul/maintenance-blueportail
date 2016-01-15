<?php

include_once("BluePHP/GUI/MenuEntryStatic.inc");

/**
 * \class MenuEntryButton
 * Entry that render a button link. This should be
 * used in order to navigate through elements or raise
 * windows without using the browser history and if 
 * you don't want to change the page content.
 */
class MenuEntryButton extends MenuEntryStatic
{
  /**
   * Create the button link
   * \param $id html id
   * \param $title button label
   * \param $params extra parameters
   */
  public function __construct($id, $title, $params = array())
  {
    $params = array_merge( array("enabled" => true), $params);

    if($params["enabled"])
      {
	$content = "<button id='$id'";
	if(isset($params["onClick"]))
	  {
	    $content .= " onclick='" . $params["onClick"] . "'";
	  }
	$content .= ">$title</button>";
      }
    else
      {
	$content = "<span class=\"Off\" id=\"${id}Off\">$title</span>";
      }
    if(isset($params["text"]))
      {
	$content .= "<p>" . $params["text"] . "</p>";
      }
    parent::__construct($content);
  }
}

?>