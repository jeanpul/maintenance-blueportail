<?php

/**
 * Calcul du temps de processing
 * en utilisant les données du flux entrant
 */
class ProcessTime_flow
{
  protected $clang;

  function __construct($clang)
  {
    $this->clang = $clang;
  }

  function process($params)
    {    
      $ptimeData = $this->clang->getProcessTimeData(array( "id" => $params["idp"] ));   
      $countData = $this->clang->getCountingData(array("id" => $ptimeData[0]["flowid"], "Entity" => "door"));
      return $this->process_from_idp($params, $params["idp"], $countData);
    }

  function process_from_idp($params, $idp, $countData)
  {
    $query = array( "idp" => $countData["idp"],
		    "Step" => $params["Step"],
		    "CompPeriod" => $params["CompPeriod"],
		    "TimeStr" => $params["TimeStr"],
		    "Entity" => "door" );
    $countValues = $this->clang->getCountersValues($query);
    $ptValues = array();
    for($i = 0; $i < count($countValues); $i++)
      {
	$count = $countValues[$i];
	$diff = $count["value0"] > $count["value1"] ? $count["value0"] - $count["value1"] : $count["value0"]; 
	if($count["value0"] == 0 and $count["value1"] == 0)
	  {
	    $pt = "undefined";
	  }
	else
	  {
	    $diff = $count["value0"] > $count["value1"] ? $count["value0"] - $count["value1"] : $count["value0"]; 
	    $pt = $diff > 0 ? round(60 / $diff) : 60;
	  }
	$ptValues[] = array( "id" => $params["idp"],
			     "value" => $pt,
			     "start" => $count["start"],
			     "end" => $count["end"],
			     "period" => $count["period"],
			     "periodInt" => $count["periodInt"] );
      }
    return $ptValues;
  }
}

?>