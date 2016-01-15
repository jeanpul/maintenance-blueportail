<?php

include_once("BluePHP/BlueCountGUI/Config.inc");

class EmptyBlockConfig
{
  function __construct($sharedCon = null)
  {
  }

  public function getEntries()
  {
    return array();
  }

  public function getEntry($params)
  {
    return $params;
  }

  public function setEntry($params)
  {
    return $params;
  }

  public function newEntry($params)
  {
    return $params;
  }

  public function delEntry($params)
  {
    return $params;
  }
}

?>