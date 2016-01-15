<?php

include_once("BluePHP/Utils/DateOps.inc");
include_once("BluePHP/Utils/DBConnect.inc");
include_once("BluePHP/BTopLocalServer/Indicator.inc");

/**
 * Calcul du temps d'attente utilisant
 * les données du numbering, le flux
 * et les valeurs precedentes
 */
class WaitingTime_pred
{
  protected $numberToTime = array();
  protected $flowSmooth = 0.6;
  protected $minFlowLevel = 5;
  protected $maxFlowLevel = 20;
  protected $minWTRate = 5;
  protected $diffFlow = true;
  protected $clang;

  function __construct($clang)
  {
    $this->clang = $clang;
    $this->setNumberToTime($this->getProcessingParams());
  }

  public function setFlowSmooth($v)
  {
    $this->flowSmooth = $v;
  }

  public function setMinFlowLevel($v)    
  {
    $this->minFlowLevel = $v;
  }

  public function setMinWTRate($r)
  {
    $this->minWTRate = $r;
  }

  public function setDiffFlow($v)
  {
    $this->diffFlow = $v;
  }

  public function getProcessingParams($params = "")
  {
    return array( "FlowSmooth" => $this->flowSmooth, 
		  "MinFlowLevel" => $this->minFlowLevel, 
		  "MinWTRate" => $this->minWTRate );
  }

  function setNumberToTime($params)
  {
    $this->numberToTime = $params;
  }

  function process($params)
    {      
      $wtimeData = $this->clang->getWaitingTimeData(array("id" => $params["idp"]));
      $numData = $this->clang->getNumberingData(array("id" => $wtimeData[0]["zoneid"], "Entity" => "all"));
      $countData = $this->clang->getCountingData(array("id" => $wtimeData[0]["flowid"], "Entity" => "door"));
      return $this->process_from_idp($params, $numData["idp"], $wtimeData[0], $numData, $countData);
    }
  
  function compute_prev_mean($data, $keyIn, $keyOut, $curPos, $length)
  {
    $mean = 0;
    $n = 0;
    for($i = $curPos; $i >= 0 && $n < $length; $i--, $n++)
      {
	$mean += max(0, $data[$i][$keyOut] - $data[$i][$keyIn]); 
      }
    if(!$n)
      {
	$mean = null;
      }
    else
      {
	$mean = $mean / $n;
      }
    return $mean;
  }

  function findFirstZeroPrev($numValues)
  {
    $i = count($numValues) - 1;
    for(; $i >= 0 && $numValues[$i]["value"] != 0; --$i)
      ;
    return $i >= 0 ? $i : 0;
  }

  function findFirstZeroNext($numValues)
  {
    $i = 0;
    for(; $i < count($numValues) - 1 && $numValues[$i]["value"] != 0; ++$i)
      ;
    return $i < count($numValues) - 1 ? $i : count($numValues) - 1;
  }

  function get_period_data(&$dataCur, $params, $numData, $countData)
  {
    $info = null;
    $query = array( "idp" => $numData["idp"],
		    "Step" => "minute",
		    "CompPeriod" => "day",
		    "TimeStr" => $params["TimeStr"],
		    "Entity" => "location" );

    $dataCur = $this->getOneDayNumberingFlowValues( array( "TimeStr" => $params["TimeStr"],
							   "idFlow" => $countData["idp"],
							   "idNum" => $numData["idp"] ) );

    if(count($dataCur))
      {
	$info = array( "start" => 0, "end" => count($dataCur),
		       "start0" => 0, "end0" => count($dataCur) );

	// to merge prev and cur day : 
	// numbering must be > 0 for the first elt of cur day
	// check to prev day last date is 1 minute less than cur day first date
	if($dataCur[0]["value"] > 0)
	  {
	    $query["TimeStr"] = addTimeFromString($params["TimeStr"], array( "day" => -1));
	    $dataPrev = $this->getOneDayNumberingFlowValues( array( "TimeStr" => addTimeFromString($params["TimeStr"], 
												   array( "day" => -1)),
								    "idFlow" => $countData["idp"],
								    "idNum" => $numData["idp"] ) );
	    if(count($dataPrev))
	      {
		$date1 = convertCommonTimeString($dataPrev[count($dataPrev) - 1]["start"]);
		$date2 = convertCommonTimeString($dataPrev[0]["start"]);
		if($date1 == addTimeFromString($date2, array( "minute" => -1)))
		  {
		    $zeroIdx = $this->findFirstZeroPrev($dataPrev);
		    $dataCur = array_merge(array_slice($dataPrev, $zeroIdx), $dataCur);
		    $info["start"] = count($dataPrev) - $zeroIdx;
		    $info["end"] = $info["end0"] = count($dataCur);
		  }
	      }
	  }

	// to merge cur and next day : 
	// numbering must be > 0 for the last elt of cur day
	// check to next day first date is 1 minute more than cur day last date
	if($dataCur[count($dataCur) - 1]["value"] > 0)
	  {
	    $query["TimeStr"] = addTimeFromString($params["TimeStr"], array( "day" => 1));
	    $dataNext = $this->getOneDayNumberingFlowValues( array( "TimeStr" => addTimeFromString($params["TimeStr"], 
												   array( "day" => -1)),
								    "idFlow" => $countData["idp"],
								    "idNum" => $numData["idp"] ) );
	    if(count($dataNext))
	      {
		$date1 = convertCommonTimeString($dataNext[0]["start"]);
		$date2 = convertCommonTimeString($dataNext[count($dataNext) - 1]["start"]);
		if($date1 == addTimeFromString($date2, array( "minute" => 1)))
		  {
		    $zeroIdx = $this->findFirstZeroNext($dataNext);
		    if($zeroIdx > 0)
		      {
			$dataCur = array_merge($dataCur, array_slice($dataNext, 0, $zeroIdx));
			$info["end0"] = count($dataCur);
		      }
		  }
	      }
	  }
      }
    return $info;
  }

  protected function isDataValid($dataElt)
  {
    return (($dataElt["value0"] != -1) &&
	    ($dataElt["value1"] != -1) &&
	    ($dataElt["value"] != -1));    
  }

  protected function getInvalidValue($params)
  {
    return array_merge($params, array( "value" => -1,
				       "valueSeconds" => 0,
				       "realWT" => 0,
				       "flow" => 0,
				       "num" => 0,
				       "flowOut" => 0,
				       "flowIn" => 0 ));
  }

  function process_from_idp($params, $idp, $wtimeData, $numData, $countData)
  {
    $dataCur = array();
    $wtValues = array();
    $info = $this->get_period_data($dataCur, $params, $numData, $countData); 

    if($info)
      {
	$dateToFind = null;
	if($params["CompPeriod"] == "minute")
	{
          $dateToFind = convertToDBTimeFormat($params["TimeStr"]);
	  // if we do not find the wanted period then we return the most recent value
	  $info["start"] = $info["end0"] - 1;
	  $info["end"] = $info["end0"];
        }

	$keyFlowOut = $wtimeData["sens"] == 1 ? "value0" : "value1";
	$keyFlowIn = $wtimeData["sens"] == 1 ? "value1" : "value0";
	$prevWT = 1;
	for($i = $info["start0"]; $i < $info["end0"]; $i++)
	  {
	    if($this->isDataValid($dataCur[$i]))
	      {
		$wtValue = $this->getInvalidValue(array( "id" => $params["idp"],							 
							 "start" => $dataCur[$i]["start"] ));
		$wt = 0; $wtSeconds = 0;
		$count = $dataCur[$i];
		$flowOut = $count[$keyFlowOut];
		$flowIn = $count[$keyFlowIn];
		$flow = $this->compute_prev_mean($dataCur, $keyFlowIn, $keyFlowOut, $i, max(1, round($prevWT * $this->flowSmooth)));
		$numSmoothed = $dataCur[$i]["value"];
		if($flow > $this->minFlowLevel)
		  {
		    $wt = $numSmoothed / $flow;
		  }
		else
		  {
		    $wt = $numSmoothed / $this->minFlowLevel;
		  }
		if($wt > $prevWT)
		  {
		    $wt = $prevWT + min($this->minWTRate, $wt - $prevWT);
		  }
		else
		  {
		    $wt = $prevWT - min($this->minWTRate, $prevWT - $wt);
		  }
		$wtSeconds = intval($wt)*60 + round(($wt - intval($wt))*60); 
		$wt = round($wt);
		$prevWT = $wt;
		$wtValue["value"] = $wt; 
		$wtValue["valueSeconds"] = $wtSeconds;
		$wtValue["flowOut"] = $flowOut;
		$wtValue["flowIn"] = $flowIn;
		$wtValue["flow"] = max(0, $flowOut - $flowIn);
		$wtValue["num"] = $dataCur[$i]["value"];
		$wtValues[] = $wtValue;

	        if(!is_null($dateToFind) and $dateToFind == $wtValue["start"])
                 {
                   $info["start"] = $i;
                   $info["end"] = $i + 1;
                 }
	      }
	    else
	      {	
		$wtValues[] = array( "id" => $params["idp"],
				     "value" => -1,
				     "valueSeconds" => 0,
				     "realWT" => 0,
				     "flow" => 0,
				     "num" => 0,
				     "flowOut" => 0,
				     "flowIn" => 0,
				     "start" => $dataCur[$i]["start"] );
	      }
	  }

	if(count($wtValues))
	  {
	    $wtValues = $this->compute_rwt($wtValues);
	  }
      }

      return array_slice($wtValues, $info["start"], $info["end"] - $info["start"], true);
  }

  function compute_rt_naif($wtValues, $num, $pos, $maxPos)
  {
    $realWT = 0;
    $sumFl = 0;
    for(; $pos < count($wtValues) && $pos <= $maxPos && $sumFl < $num; ++$pos)
      {
	if($wtValues[$pos]["num"] == 0)
	  {
	    $sumFl = $num;
	  }
	else
	  {
	    $sumFl += $wtValues[$pos]["flow"];
	  }
	++$realWT;
      }
    if($pos <= $maxPos && $sumFl < $num)
      {
	$realWT = null;
      }
    return $realWT;
  }

  function compute_rwt($wtValues)
  {    
    $wtValues[count($wtValues) - 1]["realWT"] = 0;
    for($i = count($wtValues) - 2; $i >= 0; --$i)
      {
	if($wtValues[$i]["num"] > 0)
	  {
	    $wtValues[$i]["realWT"] = $this->compute_rt_naif($wtValues, 
							     $wtValues[$i]["num"],
							     $i, 
							     $i + $wtValues[$i+1]["realWT"]);
	  }
	else
	  {
	    $wtValues[$i]["realWT"] = 0;
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
    return false;
  }

  protected function getDBFromDate($date)
  {
    $con = null;
    $DBname = substr($date, 0, 4) . substr($date, 4, 2) . substr($date, 6, 2);
    $DBname = BTOP_SERVER_DBPATH . "/days/BCL_" . $DBname . ".db";
    if(file_exists($DBname))
      {
	$con = new DBConnect(DATADBTYPE, DATADBHOST, $DBname, 
			     DATADBUSER, DATADBPASSWD);
	$con->connectToDB();
      }
    return $con;
  }

  public function computeInDB($params)
  {
    $values = $this->process($params);
    $con = $this->getDBFromDate($params["TimeStr"]);

    if(!$con->isTableExist("WaitingTime_pred_MINUTE"))
      {
	$query = "create table WaitingTime_pred_MINUTE(id INTEGER, start TIMESTAMP, " . 
	  "valueSeconds INTEGER, realWT INTEGER, flow INTEGER, flowOut INTEGER, " . 
	  "flowIn INTEGER, num INTEGER, PRIMARY KEY(start, id))";
	$con->query($query);
      }

    foreach($values as $elt)
      {
	$query = "insert or replace into " . "ZoneWaitingTime_MINUTE " . 
	  "(id, start, value, nbcumul, nbexpected) values " . 
	  "(" . $elt["id"] . ",\"" . $elt["start"] . "\"," . $elt["value"] . 
	  "," . "0,0" . ")";
	$con->query($query);

	$query = "insert or replace into " . "WaitingTime_pred_MINUTE " . 
	  "(id, start, valueSeconds, realWT, flow, flowOut, flowIn, num) values " . 
	  "(" . $elt["id"] . ",\"" . $elt["start"] . "\"," . $elt["valueSeconds"] . 
	  "," . $elt["realWT"] . "," . $elt["flow"] . "," . $elt["flowOut"] . "," . 
	  $elt["flowIn"] . "," . $elt["num"] . ")";
	$con->query($query);
      }
    $con->closeDB();
    return true;
  }

  public function getOneDayNumberingFlowValues($params)
  {
    $data = array();
    $con = $this->getDBFromDate($params["TimeStr"]);
    if($con) 
      {
	$query = "select fc.start as start, value0, value1, value " . 
	  "from FlowCounting_MINUTE as fc, ZoneNumbering_MINUTE as zn where " . 
	  "fc.id=" . $params["idFlow"] . " and zn.id=" . $params["idNum"] .
	  " and fc.start = zn.start order by start asc";
	$result = $con->query($query);   
	$data = $con->getAssociativeArray($result);
	$result->freeResult();
	$con->closeDB();
      }
    return $data;
  }

  public function getValuesInDB($params)
  {
    $data = array();
    $con = $this->getDBFromDate($params["TimeStr"]);
    $TimeStrDB = convertToDBTimeFormat($params["TimeStr"]);
    // FAB: WARNING ! see warning in Numbering_overflow
    $sel = "ZoneWaitingTime_MINUTE.id as id, ZoneWaitingTime_MINUTE.start as start, " . 
      "value, valueSeconds, realWT, flow, flowOut, flowIn, num";
    $cond = "ZoneWaitingTime_MINUTE.start=WaitingTime_pred_MINUTE.start and " .
      "ZoneWaitingTime_MINUTE.id=WaitingTime_pred_MINUTE.id and " . 
      "ZoneWaitingTime_MINUTE.id=" . $params["idp"];
    if($params["CompPeriod"] == "minute")
      {
	$cond .= " and ZoneWaitingTime_MINUTE.start=\"$TimeStrDB\"";
      }
    $order = "ZoneWaitingTime_MINUTE.start asc";
    $query = "select $sel from ZoneWaitingTime_MINUTE, WaitingTime_pred_MINUTE " . 
      "where $cond order by $order";
    $result = $con->query($query);
    $data = $con->getAssociativeArray($result);
    $result->freeResult();
    $con->closeDB();
    return $data;
  }
}

?>
