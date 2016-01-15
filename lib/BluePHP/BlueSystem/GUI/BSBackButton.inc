<?php

include_once("BluePHP/BlueSystem/GUI/BSFrame.inc");

class BSBackButton extends BSFrame
{
  private $params = null;

  public function __construct($id, $params, $root)
  {
    parent::__construct($id, "BSBackButton", $root);
    $this->params = array_merge(array( "form" => array(), 
				       "label" => "Back" ), $params);
  }

  protected function getURL()
  {
    return $this->params["URL"];
  }

  protected function getForm()
  {
    return $this->params["form"];
  }

  protected function getLabel()
  {
    return $this->params["label"];
  }
}

?>