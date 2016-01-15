<?php

include_once("BluePHP/BTopLocalServer/Indicator.inc");

/**
 * Calcul du temps d'attente utilisant
 * les données du numbering et un facteur correctif
 * en fonction de l'intervale de valeur
 */
class WaitingTime_numbering implements Indicator
{
  // waiting time from numbering
  // array ( A, B, C) 
  // A = [ left interval
  // B = right interval [ 
  // C = value returned 
  protected $numberToTime = array();

  protected $clang;

  function __construct($clang)
  {
    $this->clang = $clang;
    $this->setNumberToTime($this->getProcessingParams());
  }

  function getProcessingParams($params = "")
  {
    return array( array (  0, 5, 0 ),
		  array (  5, 10, 5 ),
		  array ( 10, 15, 10 ),
		  array ( 15, 0, 15 ) );
  }

  function setNumberToTime($params)
  {
    $this->numberToTime = $params;
  }

  function process($params)
    {      
      $wtimeData = $this->clang->getWaitingTimeData(array("id" => $params["idp"]));
      $numData = $this->clang->getNumberingData(array("id" => $wtimeData[0]["zoneid"], "Entity" => "location"));
      return $this->process_from_idp($params, $numData["idp"]);
    }

  function process_from_idp($params, $idp)
  {
    $wtimeData = $this->clang->getWaitingTimeData(array("id" => $params["idp"]));
    $numData = $this->clang->getNumberingData(array("id" => $wtimeData[0]["zoneid"], "Entity" => "location"));
    $params["idp"] = $numData["idp"];
    $numValues = $this->clang->getNumberingValues($params);    
    $countValues = $this->clang->getCountersValues( array_merge($params,
								array( "idp" => $idp,
								       "Entity" => "group" )) );
    $wtValues = array();
    for($i = 0; $i < count($numValues); $i++)
      {
	if(isset($countValues[$i]))
	  {
	    $wt = $wtSeconds = "NA";
	    $count = $countValues[$i];
	    $diff = $count["value0"] > $count["value1"] ? $count["value0"] - $count["value1"] : $count["value0"]; 
	    if($diff)
	      {
		$wt = $numValues[$i]["value"] / $diff;
	      }
	    else 
	      {
		$wt = $this->getWaitingTimeValueFromNumbering($numValues[$i]["value"]);
	      }
	    $wtSeconds = intval($wt)*60 + round(($wt - intval($wt))*60); 
	    $wt = round($wt);
	    $wtValues[] = array( "id" => $params["idp"],
				 "value" => $wt,
				 "valueSeconds" => $wtSeconds,
				 "start" => $numValues[$i]["start"],
				 "end" => $numValues[$i]["end"],
				 "period" => $numValues[$i]["period"],
				 "periodInt" => $numValues[$i]["periodInt"] );		     
	  }
      }
    return $wtValues;
  }

  function getWaitingTimeValueFromNumbering($n)
  {
    foreach($this->numberToTime as $k => $v)
      {
	if($n["value"] >= $v[0] and ($n["value"] < $v[1] or $v[1] == 0))
	  {
	    $n["value"] = $v[2];
	    return $n;
	  }
      }
    $lastV = end($this->numberToTime);
    $n["value"] = $lastV[1];
    return $n;
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
