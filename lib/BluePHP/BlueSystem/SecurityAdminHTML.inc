<?php

include_once("BluePHP/BlueSystem/GUI/BSFrame.inc");

/**
 * \class SecurityAdminHTML
 */
class SecurityAdminHTML extends BSFrame
{
  public function __construct($id, $params)
  {
    parent::__construct($id, array_merge( array( "step" => 0 ), $params));
  }
}

?>