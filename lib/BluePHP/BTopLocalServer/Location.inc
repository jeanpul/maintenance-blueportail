<?php

include_once("BluePHP/BTopLocalServer/Config.inc");
include_once("BluePHP/BTopLocalServer/Zones.inc");

class Location extends Zones
{
  public function __construct($sharedCon = null)
  {
    parent::__construct($sharedCon);
  }

  function getEntries()
  {
    return $this->Select("* from Locations");
  }
}

?>