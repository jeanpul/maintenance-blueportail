<?php

include_once("BluePHP/GUI/MenuEntryButton.inc");

/**
 * \class MenuEntryWindow
 * Entry that raises a window using the BlueSystem javascript module (ui dialog).
 * This is implemented using a button tag and the onclick callback
 * BlueSystem.ModuleWindowStart
 */
class MenuEntryWindow extends MenuEntryButton
{
  /**
   * Create the button link
   * \param $id html id
   * \param $hrefparam url query string parameters passed to the javascript dialog
   * \param $windowParams array of options passed to the javascript method BlueSystem.ModuleWindowStart
   * \param $title New window title
   */
  public function __construct($id, $hrefparam, $windowParams, $title, $extra = null)
  {
    $wtype = "ModuleWindowStart";
    if($extra and isset($extra["wtype"]))
      {
	$wtype = $extra["wtype"];
      }

    parent::__construct($id, $title,
			array( "onClick" => "BlueSystem.$wtype(" .
			       json_encode($windowParams) . ",\"$hrefparam\");" ));
  }
}


?>