<?php

include_once("BluePHP/BlueSystem/GUI/BSFrame.inc");

class BCMFrame extends BSFrame
{
  public function __construct($id, $params = null, $root = null,
			      $location = "BluePHP/BTopLocalServer/templates")
  {
    parent::__construct($id, $params, $root, $location);
  }
}

?>