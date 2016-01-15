<?php

include_once("BluePHP/BlueSystem/GUI/BSFrame.inc");

/**
 * \class NetworkConfigurationHTML
 */
class NetworkConfigurationHTML extends BSFrame
{
  public function __construct($id, $params)
  {
    parent::__construct($id, array_merge( array( "step" => 0,
						 "NETDEV" => "eth0" ), $params));
  }
}

?>