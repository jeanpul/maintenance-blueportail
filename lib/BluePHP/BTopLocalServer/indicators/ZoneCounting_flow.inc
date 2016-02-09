<?php

include_once("BluePHP/BTopLocalServer/IndicatorBase.inc");

/**
 * Moyenne entre les entrees et les sorties
 * a partir de la periode jour sinon
 * retourne les donnees de la base
 */
class ZoneCounting_flow extends IndicatorBase
{
  public function __construct($clang)
  {
    parent::__construct($clang, "ZoneCounting");
  }
}

?>
