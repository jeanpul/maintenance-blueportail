<?php

include_once("BluePHP/DateOps.inc");

/**
 * Calcul par default,
 * retourne directement les donnees de la base
 */
class FlowCounting_module
{
  protected $clang;

  function __construct($clang)
  {
    $this->clang = $clang;    
  }

  function process($params = "")
  {
    $params["valueKind"] = "counting";
    if(!isset($params["TimeStamp"]))
      {
	if(isset($params["TimeStr"]))
	  {
	    $params["TimeStamp"] = mktimeFromString($params["TimeStr"]);
	  }
	else
	  {
	    // FAB : must be UTC
	    $params["TimeStamp"] = time();
	  }
      }
    $params["TimeStamp"] = mktimeFromString(strftime("20100104%H%M00", $params["TimeStamp"]));
    return $this->clang->getValues($params);
  }
}

?>
