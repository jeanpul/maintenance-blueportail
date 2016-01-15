<?php

include_once("BluePHP/Utils/ModuleProcessing.inc");
include_once("BluePHP/Utils/DateOps.inc");
include_once("BluePHP/BTopLocalServer/Config.inc");
include_once("BluePHP/BTopLocalServer/BTopSensors.inc");
include_once("BluePHP/BTopLocalServer/BQueueSensors.inc");
include_once("BluePHP/BTopLocalServer/NumberingAvailable.inc");
include_once("BluePHP/BTopLocalServer/GlobalProperties.inc");

class GrabIPTable extends ModuleProcessing
{
  public function __construct($sharedCon = null)
  {
    parent::setParams( array( "name" => "GrabIPTable",
			      "keyname" => "ref",
			      "sort" => "ref" ),
		       $sharedCon);
  }

  public function update($params)
  {
    $query = "insert or replace into GrabIPTable (ref,lasttimestamp,laststatus,lastIP,lastvalue) values " . 
      "('" . $params["ref"] . "','" . $params["lasttimestamp"] . "','" . $params["laststatus"]  . "'," . 
      "'" . $params["lastIP"] . "'," . $params["lastvalue"] . ")";
    parent::QueryNoReturn($query);
    return $params;
  }

  public function getEntriesBTops()
  {
    return $this->Select("* from GrabIPTable where ref like '%B-TOP%'");
  }

  public function getEntriesBQueues()
  {
    return $this->Select("* from GrabIPTable where ref like '%B-QUEUE%'");
  }

  protected function checkEntriesForAllSensors($sensorModule, $receivedSensors, $defaultInfo)
  {
    $inThresh = 5;

    $defaultStatus = array( "lasttimestamp" => null, "laststatus" => "never received", 
			    "lastIP" => null, "lastvalue" => null );
   
    // get all declared sensors and index them by
    // their reference
    $allSensors = array();
    $sensors = $sensorModule->getEntries();
    foreach($sensors as $v)
      {
	$allSensors[$v["ref"]] = array_merge( $defaultStatus, $v );
      }   

    $module = new GlobalProperties($this);
    $lastProcessTime = $module->getLastProcessedTimeString();

    // grab all received sensors then merge    
    foreach($receivedSensors as $v)
      {
	if(array_key_exists($v["ref"], $allSensors))
	  {
	    $lasttimestamp = $v["lasttimestamp"];
	    $age = getDateSub($lastProcessTime, convertCommonTimeString($lasttimestamp));
	    if(($age/60) > $inThresh) 
	      {
		$v["laststatus"] = "last reception > $inThresh minutes";
	      } 
	    $allSensors[$v["ref"]] = array_merge( $allSensors[$v["ref"]], $v );
	  }
	else
	  {
	    $allSensors[$v["ref"]] = array_merge( $defaultInfo, $v );
	  }
      }

    return $allSensors;
  }

  public function checkEntriesForAllBTops()
  {
    return $this->checkEntriesForAllSensors(new BTopSensors($this), 
					    $this->getEntriesBTops(),
					    array( "door" => null ) );
  }

  public function checkEntriesForAllBQueues()
  {
    return $this->checkEntriesForAllSensors(new BQueueSensors($this), 
					    $this->getEntriesBQueues(),
					    array( "location" => null ) );
  }
}

?>