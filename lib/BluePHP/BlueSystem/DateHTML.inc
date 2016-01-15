<?php

include_once("BluePHP/BlueSystem/GUI/BSFrame.inc");
include_once("BluePHP/BlueSystem/GUI/BSDialogInfo.inc");
include_once("BluePHP/BlueSystem/Date.inc");

/**
 * \class DateHTML
 */
class DateHTML extends BSFrame
{
  public function __construct($id, $params)
  {
    parent::__construct($id, array_merge(array( "step" => 0 ), $params));
  }

  public function submit()
  {
    $params = $this->getParams();
    $module = new Date();
    $module->setEntry($params);
    $dialog = new BSDialogInfo("Date_done",
			       array( "title" => "Time and date settings",
				      "msg" => "Time and date changed."));
    $dialog->setModal(true);
    $dialog->setDraggable(false);
    $dialog->setResizable(false);
    $dialog->render();
  }
}

?>