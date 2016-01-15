<?php


include_once("BluePHP/Utils/DBConnect.inc");
include_once("BluePHP/Utils/DateOps.inc");
include_once("BluePHP/BTopLocalServer/Config.inc");
include_once("BluePHP/BTopLocalServer/GlobalProperties.inc");
include_once("BluePHP/BTopLocalServer/Indicator.inc");
include_once("BluePHP/BTopLocalServer/NumberingProcessing.inc");
include_once("BluePHP/BTopLocalServer/ZoneCounting.inc");
include_once("BluePHP/BTopLocalServer/NumberingReset.inc");

/**
 * Compute the numbering by using :
 * - the previous numbering value
 * - the current difference between in and out from the
 *   ZoneCounting indicator
 */
class Numbering_zonecounting implements Indicator
{
  protected $clang;  

  function __construct($clang = null)
  {
    $this->clang = $clang;
  }

  public function process($params)
  {      
    if(!isset($params["TimeStr"]))
      {
	$module = new GlobalProperties();
	$data = $module->getLastPushTask();
	$params["TimeStr"] = $data[0]["value"];
	$timestamp = subTime(mktimeFromString($params["TimeStr"]),
			     array( "hour" => 0,
				    "minute" => 1,
				    "second" => 0,
				    "month" => 0,
				    "day" => 0,
				    "year" => 0 ));
	$params["TimeStr"] = strftime("%Y%m%d%H%M%S", $timestamp);
      }

    $numValues = array();
    $module = new NumberingProcessing();
    $numEntry = $module->getEntry(array( "id" => $params["idp"] ));
    $zcModule = new ZoneCounting($module);
    $zcEntry = $zcModule->getEntryFromZone(array("zone" => $numEntry["zone"]));
    $nrModule = new NumberingReset();
    $nrValues = $nrModule->getValues(array( "TimeStr" => $params["TimeStr"],
					    "id" => $params["idp"] ));

    $query = array( "id" => $zcEntry["id"],
		    "Step" => "minute",
		    "CompPeriod" => "day",
		    "TimeStr" => $params["TimeStr"],
		    "Entity" => "location" );
    $sqlTimeStr = convertToDBTimeFormat($params["TimeStr"]);
    $selectedHour = isset($params["HourValue"]) ? $params["HourValue"] : substr($params["TimeStr"], 8, 2);
    $zcValues = $zcModule->getValues($query);
    $numValuePrev = 0;

    foreach($zcValues as $zcValue)
      {	
	$value = 0;
	if(isset($nrValues[$zcValue["start"]]))
	  {
	    $value = $nrValues[$zcValue["start"]]["value"];
	  }
	else
	  {
	    $value = $numValuePrev + $zcValue["value0"] - $zcValue["value1"];
	  }
	$numValue = array( "id" => $numEntry["id"],
			   "start" => $zcValue["start"],
			   "end" => $zcValue["start"],
			   "value" => $value
			   );
	$numValuePrev = $value;

	if($params["CompPeriod"] == "minute")
	  {
	    if($sqlTimeStr == $numValue["start"])
	      {
		$numValues[] = $numValue;
		break;
	      }
	  }
	else if($params["CompPeriod"] == "hour")
	  {
	    $curHour = substr($numValue["start"], 11, 2);
	    if($selectedHour == $curHour)
	      {
		$numValues[] = $numValue;
	      }
	  }
	else
	  {
	    $numValues[] = $numValue;
	  }
      }

    return $numValues;
  }

  /**
   * compute and return the set of indicator value 
   */
  public function computeInDB($params)
  {
    $data = array();

    $module = new NumberingProcessing();
    $elts = $module->getEntries();
    
    $timestr = $module->getLastPushTask();
    $curTimeStamp = subTime(mktimeFromString($timestr),
			     array( "hour" => 0,
				    "minute" => 1,
				    "second" => 0,
				    "month" => 0,
				    "day" => 0,
				    "year" => 0 ));
			    
    $prevTimeStamp = subTime($curTimeStamp,
			     array( "hour" => 0,
				    "minute" => 1,
				    "second" => 0,
				    "month" => 0,
				    "day" => 0,
				    "year" => 0 ));
    foreach($elts as $k => $v)
      {
	if($v["numb_type"] == "Numbering_zonecounting")
	  {
	    // retrieve numbering data from the previous minute
	    $dataNumb = $module->getOneMinuteValues(array("TimeStamp" => $prevTimeStamp,
						      "id" => $v["id"]));

	    if(is_array($dataNumb) and count($dataNumb))
	      {
		// retrieve zone counting data from the current minute	    
		$zcModule = new ZoneCounting($module);
		$zc = $zcModule->getEntryFromZone(array("zone" => $v["zoneid"]));
		
		$dataCount = $zcModule->getOneMinuteValues(array("TimeStamp" => $curTimeStamp, 
								 "id" => $zc["id"]));

		if(is_array($dataCount) and count($dataNumb))
		  {
		    // the current numbering is
		    // numbering(t) = numbering(t-1) + in(t) - out(t)
		    $value = $dataNumb[0]["value"] + $dataCount[0]["value0"] - $dataCount[0]["value1"];
		    
		    $data[] = array( "table" => "ZoneNumbering_MINUTE",
				     "id" => $v["id"],
				     "start" => strftime("%Y-%m-%d %H:%M:%S", $curTimeStamp),
				     "value" => $value,
				     "nbcumul" => 0,
				     "nbexpected" => 0 );
		  }
	      }
	  }
      }
    $this->updateComputedValues($data);
    return $data;
  }

  // store new computed values into the appropriate db
  public function updateComputedValues($data)
  {
    foreach($data as $elt)
      {
	// retrieve the dabase name from the timestamp specified
	$DBname = substr($elt["start"], 0, 4) . substr($elt["start"], 5, 2) . substr($elt["start"], 8, 2);
	$DBname = BTOP_SERVER_DBPATH . "/days/BCL_" . $DBname . ".db";
	$con = new DBConnect(DATADBTYPE, DATADBHOST, $DBname, 
			     DATADBUSER, DATADBPASSWD);
	$con->connectToDB();
	$query = "insert or replace into " . $elt["table"] . 
	  " (id, start, value, nbcumul, nbexpected) values " . 
	  "(" . $elt["id"] . ",\"" . $elt["start"] . "\"," . $elt["value"] . "," . 
	  $elt["nbcumul"] . "," . $elt["nbexpected"] . ")";
	$res = $con->query($query);
	$con->closeDB();
      }
  }

  public function computedByPushAndProcess()
  {
    return false;
  }

  public function getValuesInDB($params)
  {
    return $this->process($params);
  }
}


?>