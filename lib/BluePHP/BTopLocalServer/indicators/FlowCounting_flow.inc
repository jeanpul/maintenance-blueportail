<?php

/**
 * Calcul par default,
 * retourne directement les donnees de la base
 */
class FlowCounting_flow
{
  protected $clang;

  function __construct($clang)
  {
    $this->clang = $clang;
  }

  function process($params = "")
  {
    $params["valueKind"] = "counting";
    return $this->clang->getValues($params);
  }

  function getValues($idp, $timestamp, $hstart, $hend)
  {
    $data = null;
    $dataDB = $this->clang->accessDataDB("minute", $timestamp);
    if($dataDB !== FALSE)
      {
	$query = "select * from FlowCounting_MINUTE where id=$idp and " . 
	  "strftime('%H%M', start) >= '$hstart' and " . 
	  "strftime('%H%M', start) < '$hend'";
	$data = $this->clang->queryDataDB($query);
      }
    return $data;
  }
}

?>
