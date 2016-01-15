<?php

include_once("BluePHP/GUI/MenuEntryStatic.inc");

/**
 * \class MenuEntryLink
 * Simple MenuEntry wich returns
 * the content specified at construction.
 * If "enabled" is set to false then a span 
 * is used instead of the anchor.
 * If "newWindow" is set to true then a click
 * on the entry will open a new navigator window.
 * \todo should be merged with MenuBarEntry
 */
class MenuEntryLink extends MenuEntryStatic
{
  /**
   * Create the link
   * \param $id html id
   * \param $link href string
   * \param $title href displayed label
   * \param $params parameter array such as "enabled" and "newWindow"
   */
  public function __construct($id, $link, $title, $params = array())
  {
    $params = array_merge(array( "enabled" => true,
				 "newWindow" => false ), 
			  $params);
    if($params["enabled"])
      {
	$extra = "";
	if($params["newWindow"])
	  {
	    $extra = " target=\"_blank\" rel=\"nofollow\" ";
	  }
	$content = "<a id=\"$id\" $extra href=\"$link\" title=\"$title\">$title</a>";
      }
    else
      {
	$content = "<span class=\"Off\" id=\"${id}Off\" title=\"$title\">$title</span>";
      }
    if(isset($params["text"]))
      {
	$content .= "<p>" . $params["text"] . "</p>";
      }
    parent::__construct($content);
  }

}

?>