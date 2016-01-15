<?php

include_once("BluePHP/BlueSystem/GUI/BSDialogInfo.inc");

/**
 * \class LogsHTML
 */
class LogsHTML extends BSDialogInfo
{
  public function __construct($id, $params)
  {
    parent::__construct($id, array(), $params);
    parent::setContent("BSDialogInfo");
  }

  public function render()
  {
    $this->setModal(true);
    $this->setTitle("Logs");
    $this->setMsg("<p class=\"info\">Please use the terminal application from the tool bar and" . 
		  " check the files in the /var/log directory</p>");
    parent::render();
  }
}

?>