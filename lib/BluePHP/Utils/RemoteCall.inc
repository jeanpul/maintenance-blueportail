<?php

class RemoteCall
{
  private $params = array();

  public function __construct($address, $method, $params)
  {
    $this->params = array("__address" => $address,
			  "__method" => $method,
			  "params" => $params );		  
  }
}

?>