<?php

include_once("BluePHP/BTopLocalServer/Indicator.inc");

/**
 * Calcul du temps d'attente utilisant
 * les données du numbering et du comptage.
 * Calcul le plus basique :
 * T = Numbering / flux
 */
class WaitingTime_flow implements Indicator
{
  protected $clang;

  function __construct($clang)
  {
    $this->clang = $clang;
  }

  function process($params = "")
  {      
    $params["Entity"] = "area";
    $params["valueKind"] = "waitingTime";
    return $this->clang->getValues($params);
  }

  public function computedByPushAndProcess()
  {
    return true;
  }

  public function computeInDB($params)
  {
    return true;
  }

  public function getValuesInDB($params)
  {
    return $this->process($params);
  }
}

?>