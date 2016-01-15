<?php

include_once("BluePHP/Utils/ModuleProcessing.inc");
include_once("BluePHP/System/NetworkInterface.inc");
include_once("BluePHP/System/Network.inc");

/**
 * \class NetworkConfiguration
 * Change the network parameters of the system.
 * Uses the Network class from the System module.
 */
class NetworkConfiguration extends ModuleProcessing
{
  private $net = null;
  private $ifaces = array();
  private $autos = array();
  private $dns = array();

  /**
   * Use the Network module
   * \param $sharedCon not used
   */
  public function __construct($sharedCon = null)
  {
    parent::__construct(array());
    $this->net = new Network();
  }

  function getEntry($params)
  {
    return $this->net->getValues();
  }

  function setEntry($params)
  {
    $this->net->setNetParams(array_merge( $this->net->getNetParams(),
					  $params) );
    $this->net->setValues();
  }

};

?>