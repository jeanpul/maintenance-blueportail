<?php

include_once("BluePHP/System/System.inc");

class Reboot 
{
  public function __construct()
  {
  }

  public function doReboot()
  {
    $system = new System();
    $system->reboot();    
  }

};

?>
