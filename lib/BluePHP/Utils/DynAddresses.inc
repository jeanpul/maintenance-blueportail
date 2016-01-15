<?php

class DynAddresses
{
  private $path = null;
  private $addresses = null;

  public function __construct($path)
  {
    $this->path = $path;
  }

  public function getClass($address)
  {
    return $this->getProperty($address, "class");
  }

  public function getLabel($address)
  {
    return $this->getProperty($address, "label");
  }

  public function getInfo($address)
  {
    return $this->getProperty($address, "info");
  }

  private function getProperty($address, $prop)
  {
    $val = null;
    if($this->addresses and isset($this->addresses[$address]))
      {
	$val = $this->addresses[$address][$prop];
      }
    else
      {
	$this->incFileFromAddress($address);
	if($this->addresses and isset($this->addresses[$address]))
	  {
	    $val = $this->addresses[$address][$prop];
	  }
      }
    return $val;
  }

  private function incFileFromAddress($address)
  {
    $found = false;
    $pathElts = explode(".", $address);
    while(!$found and $address)
      {
	$filename = $this->path . "/" . $address . ".inc";
	if(file_exists($filename))
	  {
	    include($filename);
	    $found = true;
	    //	    print_r($filename . " found \n");
	  }
	else
	  { // remove the last part of the address	   
	    $pos = strrpos($address, ".");
	    if($pos === false)
	      {
		$address = false;
	      }
	    else
	      {
		$address = substr($address, 0, $pos);
	      }
	  }
      }
    return $found;
  }

}

?>