<?php

include_once("BluePHP/System/Network.inc");
include_once("BluePHP/BlueSystem/GUI/BSWindow.inc");

/**
 * \class NetworkConfigurationWindow
 * \deprecated should be removed
 */
class NetworkConfigurationWindow extends BSWindow
{
  private $device = null;
  private $network = null;

  public function __construct($device, $root)
  {
    parent::__construct(array( "main" => _("BlueEyeVideo system - Network Configuration"),
			       "sub" => _("Network") ),
			"NetworkConfiguration", 
			$root,
			"BluePHP/BlueSystem/GUI/templates/");
    $this->device = $device;
    $this->network = new Network();
  }

  protected function getValues()
  {
    return $this->network->getValues();
  }

}

?>