<?php

include_once("BluePHP/Utils/DynAddresses.inc");

class BSAddresses extends DynAddresses
{
  private $module = null;

  public function __construct()
  {
    parent::__construct("/home/DATA/BlueSystem/");
  }
}

?>