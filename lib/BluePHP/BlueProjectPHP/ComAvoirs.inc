<?php

include_once("BluePHP/BlueProjectPHP/Config.inc");
include_once("BluePHP/BlueProjectPHP/ComFactures.inc");

class ComAvoirs extends ComFactures
{
  public function __construct($sharedCon = false)
  {
    parent::__construct($sharedCon); 
    $this->setPrefix("AV");
  }

}

?>