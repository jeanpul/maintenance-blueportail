<?php

/**
 */
class FlowCounting_bizone
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

  function process_with_idp($params = "", $idFlow, $idN1, $idN2)
  {   
    // Porte WEST, FAT WEST
    $query = array( "idp" => $idFlow,
		    "Step" => $params["Step"],
		    "CompPeriod" => $params["CompPeriod"],
		    "TimeStr" => $params["TimeStr"],
		    "Entity" => "door" );

    // recuperation des valeurs de comptage
    $countValues = $this->clang->getCountersValues($query, true);

    // recuperation denombrement 
    $query["Entity"] = "location";
    $query["idp"] = $idN1;
    $numValuesWest = $this->clang->getNumberingValues($query);

    // recuperation denombrement 
    $query["Entity"] = "location";
    $query["idp"] = $idN2;
    $numValuesEast = $this->clang->getNumberingValues($query);    

    for($i = 0; $i < count($countValues); ++$i)
      {
	$f0 = isset($countValues[$i]["value0"]) ? $countValues[$i]["value0"] : 0;
	$f1 = isset($countValues[$i]["value1"]) ? $countValues[$i]["value1"] : 0;
	$n1 = isset($numValuesWest[$i]["value"]) ? $numValuesWest[$i]["value"] : 0;
	$n2 = isset($numValuesEast[$i]["value"]) ? $numValuesEast[$i]["value"] : 0;
	if($n1 != 0 or $n2 != 0)
	  {
	    if($f0 != 0)
	      {
		$countValues[$i]["value0"] = (int) $f0*$n1/($n1+$n2);
	      }
	    if($f1 != 0)
	      {
		$countValues[$i]["value1"] = (int) $f1*$n1/($n1+$n2);
	      }
	  }
	else
	  {
	    $countValues[$i]["value0"] = (int) 0.5*$f0;
	    $countValues[$i]["value1"] = (int) 0.5*$f1;
	  }
      }
    return $countValues;
  }

}

?>
