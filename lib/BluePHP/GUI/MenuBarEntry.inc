<?php

include_once("BluePHP/BlueSystem/GUI/BSApplication.inc");
include_once("BluePHP/GUI/MenuBarLink.inc");
include_once("BluePHP/GUI/MenuEntryStatic.inc");

/**
 * \class MenuBarEntry
 * Entry that used by the jquery.menubar plugin. This
 * menu entry uses a anchor with a dashed href containing
 * the menu parameters so you can identity the destination.
 * If the entry is not enabled then it's replaced by a span
 * with the class "Off" and the id replaced by "idOff"
 * TODO : must create a BSMenuBarEntry
 */
class MenuBarEntry extends MenuEntryStatic
{
  /**
   * Create the entry
   * \param $id html tag
   * \param $menuParams array which is the menu destination as a http_build_query
   * \param $title displayed label
   * \param $params optional extra parameters which specify if the entry is "enabled" or not
   */
  public function __construct($id, $menuParams, $title, $params = array())
  {
    $params = array_merge(array( "enabled" => true ), $params);
    if($params["enabled"])
      {
	$content = getMenuBarLink($id, $menuParams, $title);
      }
    else
      {
	$content = "<span class=\"Off\" id=\"${id}Off\">$title</span>";
      }
    if(isset($params["text"]))
      {
	$content .=  "<p>" . $params["text"] . "</p>";
      }
    parent::__construct($content);
  }
}


?>