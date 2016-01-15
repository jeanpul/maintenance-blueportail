<?php

/**
 * Moyenne entre les entrees et les sorties
 * a partir de la periode jour sinon
 * retourne les donnees de la base
 */
class ZoneCounting_flow
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
