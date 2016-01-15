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
}

?>
