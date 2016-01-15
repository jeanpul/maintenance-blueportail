<?php

include_once("BluePHP/BTopLocalServer/Indicator.inc");

/**
 * Calcul du numbering en utilisant
 * directement les donnees du B-Queue
 */
class Numbering_queue implements Indicator
{
  protected $clang;

  public function __construct($clang)
  {
    $this->clang = $clang;
  }
  
  public function process($params = "")
  {      
    $params["valueKind"] = "numbering";

    $p =  $this->clang->getValues($params);
    return $p;
  }

  public function computedByPushAndProcess()
  {
    return true;
  }

  public function computeInDB($params)
  {
    return false;
  }

  public function getValuesInDB($params)
  {
    return $this->process($params);
  }
}

?>
