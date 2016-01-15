<?php

include_once("BluePHP/BlueSystem/GUI/BSFrame.inc");

class BQueueFrame extends BSFrame
{
  public function __construct($id, $params = null, $root = null,
			      $location = "BluePHP/BQBH/Config/templates")
  {
    parent::__construct($id, $params, $root, $location);
  }
}

?>