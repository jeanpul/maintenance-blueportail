<?php

include_once("BluePHP/BlueSystem/GUI/BSFrame.inc");

class ComFrame extends BSFrame
{
  public function __construct($id, $params = null, $root = null,
			      $location = "BluePHP/Communication/templates")
  {
    parent::__construct($id, $params, $root, $location);
  }
}

?>