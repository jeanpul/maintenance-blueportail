<?php

include_once("BluePHP/BlueSystem/GUI/ActiveWindow.inc");

/**
 * \class DataWindow
 * Manage how to render data into the active window
 */
class DataWindow extends ActiveWindow
{
  private $dataAction = null;

  public function __construct($id, $params, $dataAction)
  {
    parent::__construct($id, get_class(), "BluePHP/BlueSystem/GUI/templates");
    $this->setDataAction($dataAction);
  }

  protected function setDataAction($a)
  {
    $this->dataAction = $a;
  }

  protected function getDataAction()
  {
    return $this->dataAction;
  }

  protected function renderContent()
  {
    $a = $this->getDataAction();
    if($a)
      {
	$a->render();
      }
  }
}

?>