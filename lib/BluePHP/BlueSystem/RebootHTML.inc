<?php

include_once("BluePHP/BlueSystem/GUI/BSFrame.inc");
include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/BlueSystem/Reboot.inc");

/**
 * \class RebootHTML
 */
class RebootHTML extends BSFrame
{
  public function __construct($id, $params)
  {
    parent::__construct($id, array_merge( array( "step" => 0 ), $params));
  }

  protected function doReboot()
  {
    $module = new Reboot();
    $module->doReboot();
  }
}

?>