<?php

include_once("BluePHP/BlueSystem/GUI/BSFrame.inc");

class BSForm extends BSFrame
{
  private $formContent = "";

  public function __construct($obj, $formContent, $params)
  {
    parent::__construct(get_class($obj), $params);
    $this->formContent = $formContent;
  }

  public function getFormContent()
  {
    return $this->formContent;
  }
}

?>