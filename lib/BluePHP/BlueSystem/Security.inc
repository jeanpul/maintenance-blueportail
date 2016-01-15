<?php

include_once("BluePHP/Utils/ModuleProcessing.inc");

class Security extends ModuleProcessing
{
  public function __construct($sharedCon = null)
  {
    parent::__construct(array());
  }

  function getEntry($params)
  {
    return array();
  }

  function setEntry($params)
  {
    $res = array();
    exec("(source /etc/BEV/scripts/security/security.inc ; Security_addUser admin " . $params["password"] . 
	 " ; Security_httpServerRestart )");
    return $res;
  }

};

?>