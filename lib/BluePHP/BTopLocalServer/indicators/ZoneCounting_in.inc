<?php

/**
 * Retourne les entrees
 */
class ZoneCounting_in
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
