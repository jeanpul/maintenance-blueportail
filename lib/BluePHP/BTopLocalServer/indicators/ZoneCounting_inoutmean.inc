<?php

/**
 * Moyenne entre les entrees et les sorties
 * a partir de la periode jour sinon
 * retourne les donnees de la base
 */
class ZoneCounting_inoutmean
{
  protected $clang;

  function __construct($clang)
  {
    $this->clang = $clang;
  }

  function process($params = "")
  {
    $params["valueKind"] = "counting";
    $values = $this->clang->getValues($params);
    if(in_array($params["CompPeriod"], array("day", "month", "year")))
      {
	$meanValues = array();
	foreach($values as $k => $v)
	  {
	    $mean = ($v["value0"] + $v["value1"]) / 2;
	    $v["value0"] = $mean;
	    $v["value1"] = $mean;
	    $meanValues[$k] = $v;
	  }
	return $meanValues;
      }
    return $values;
  }
}

?>
