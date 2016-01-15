<?php

include_once("BluePHP/GUI/MenuEntryStatic.inc");

class UIButton extends MenuEntryStatic
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
	$content = "<button type='button' class='ui-button ui-widget' id='$id'";
	if(isset($params["onClick"]))
	  {
	    $content .= " onclick='" . $params["onClick"] . "'";
	  }
	$content .= "><span class='ui-button-text'>$title</span></button>";
      }
    parent::__construct($content);
  }
}

?>