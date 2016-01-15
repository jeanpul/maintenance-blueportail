<?php

include_once("BluePHP/BTopLocalServer/Config.inc");
include_once("BluePHP/Utils/ModuleProcessing.inc");

class ClearSettings extends ModuleProcessing
{
  public function __construct($sharedCon = null)
  {
    parent::__construct(array());
  }

  function getEntry($params)
  {
    return array();
  }

  function setEntry($params)
  {
    exec("/etc/BEV/BlueCount/default/createAllTables.sh " . BTOP_SERVER_DBPATH);
  }

}

?>