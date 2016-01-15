<?php

include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/BlueSystem/GUI/BSApplication.inc");
include_once("BluePHP/BTopLocalServer/ClearSettings.inc");
include_once("BluePHP/BlueSystem/GUI/BSDialogSubmit.inc");
include_once("BluePHP/BlueSystem/GUI/BSDialogInfo.inc");
include_once("BluePHP/BTopLocalServer/BCMFrame.inc");

/**
 * \class ClearSettingsHTML
 */
class ClearSettingsHTML extends BCMFrame
{
  public function __construct($id, $params)
  {
    parent::__construct($id, array_merge(array("step" => 0), $params));
  }

  public function submit()
  {
    $module = new ClearSettings();
    $module->setEntry(array());
    $dialog = new BSDialogInfo("ClearSettings_done",
			       array( "title" => "Settings cleared.",
				      "msg" => "Settings cleared done."));
    $dialog->setModal(true);
    $dialog->setDraggable(false);
    $dialog->setResizable(false);
    $dialog->render();
  }
}

?>