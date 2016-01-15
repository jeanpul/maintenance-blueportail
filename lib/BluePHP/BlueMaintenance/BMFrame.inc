<?php

include_once("BluePHP/BlueSystem/GUI/BSFrame.inc");

class BMFrame extends BSFrame
{
  public function __construct($id, $params = null, $root = null,
			      $location = "BluePHP/BlueMaintenance/templates")
  {
    parent::__construct($id, $params, $root, $location);
  }
}

?>