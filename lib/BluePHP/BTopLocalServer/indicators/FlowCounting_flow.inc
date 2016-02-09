<?php

include_once("BluePHP/BTopLocalServer/IndicatorBase.inc");

/**
 * Calcul par default,
 * retourne directement les donnees de la base
 */
class FlowCounting_flow extends IndicatorBase
{
  public function __construct($clang)
  {
    parent::__construct($clang, "FlowCounting");
  }
}

?>
