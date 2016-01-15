<?php

include_once("BluePHP/BTopLocalServer/BCMXMLFileClient.inc");

class BCMServerClient extends BCMXMLFileClient
{
  private $host = false;
  private $cbp = "";

  function BCMServerClient($host, $cbp)
  {
    parent::__construct();
    $this->host = $host;
    $this->cbp = $cbp;
  }

  protected function getFilename($f, $extraParams = null)
  {
    $url = "http://" . $this->host . 
      "/BTopLocalServer/BCMServer.php?function=" . $f . ($this->cbp != "" ? "&clientBluePortail=" . $this->cbp : "") . 
      (!is_null($extraParams) ? "&$extraParams" : "");
    return $url;
  }
}

?>