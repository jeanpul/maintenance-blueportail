<?php

include_once("BluePHP/BlueSystem/Tools/SensorInterface.inc");

class BQBHSensor implements SensorInterface
{
  public function getStatus() 
  {
    return true;
  }
}

?>