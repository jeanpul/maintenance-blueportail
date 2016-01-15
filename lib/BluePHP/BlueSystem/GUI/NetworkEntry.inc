<?php

include_once("BluePHP/System/Network.inc");
include_once("BluePHP/GUI/ComponentTemplate.inc");
include_once("BluePHP/GUI/MenuEntryState.inc");

/**
 * \class NetworkEntry
 * Dynamic menu entry for
 * network parameters
 * \todo check if this is used or not
 */
class NetworkEntry extends ComponentTemplate implements MenuEntryState
{
  private $device = null;
  private $network = null;

  public function __construct($device = "eth0")
  {
    parent::__construct("NetworkEntry", "BluePHP/BlueSystem/GUI/templates/");
    $this->device = $device;
    $this->network = new Network();
  }

  public function isEnabled()
  {
    return $this->network->checkDevice($this->device);
  }
}

?>

