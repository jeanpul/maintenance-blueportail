<?php

include_once("BluePHP/BlueSystem/GUI/BSFrame.inc");

class BPFrame extends BSFrame
{
  public function __construct($id, $params = null, $root = null,
			      $location = "BluePHP/BlueProjectPHP/templates")
  {
    parent::__construct($id, $params, $root, $location);
  }
}

?>