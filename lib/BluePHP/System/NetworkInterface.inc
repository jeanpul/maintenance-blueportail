<?php

class NetworkInterface
{
  private $params = array( "dev" => null,
			   "address" => null,
			   "netmask" => null,
			   "gateway" => null,
			   "ether" => null );
  
  public function __construct($dev)
  {
    $this->params["dev"] = $dev;
  }

  public function setAddress($address)
  {
    $this->params["address"] = $address;
  }

  public function setNetMask($netmask)
  {
    $this->params["netmask"] = $netmask;
  }

  public function setGateway($gateway)
  {
    $this->params["gateway"] = $gateway;
  }

  public function setParameter($name, $value)
  {
    $this->params[$name] = $value;
  }

  public function getParameter($name)
  {
    return $this->params[$name];
  }   

  public function getDev()
  {
    return $this->params["dev"];
  }
}

?>
