<?php

class Addresses
{
  private $file = null;
  private $addresses = null;

  public function __construct($file)
  {
    $this->file = $file;
    if(file_exists($this->file))
      {
	include_once($this->file);
      }
  }

  public function getClass($address)
  {
    return $this->getProperty($address, "class");
  }

  public function getLabel($address)
  {
    return $this->getProperty($address, "label");
  }

  private function getProperty($address, $prop)
  {
    return $this->addresses ? $this->addresses[$address][$prop] : null;
  }
}

?>