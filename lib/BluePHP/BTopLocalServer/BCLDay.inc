<?php

include_once("BluePHP/Utils/ModuleProcessing.inc");
include_once("BluePHP/BTopLocalServer/Config.inc");
include_once("BluePHP/BTopLocalServer/WaitingTimeProcessing.inc");
include_once("BluePHP/BTopLocalServer/NumberingProcessing.inc");

class BCLDay extends ModuleProcessing
{
  private $day = null;

  public function __construct($day = null)
  {
    $this->day = $day;
    if(is_null($this->day))
      {
	$this->day = strftime("%Y%m%d");
      }
    $this->setConParams( array( "type" => DATADBTYPE,
				"host" => DATADBHOST,
				"name" => DATADBPATH . "/days/BCL_" . $this->day . ".db",
				"user" => DATADBUSER,
				"passwd" => DATADBPASSWD )
			 );
  }

  private function get_status_bi($tableName)
  {
    return parent::Select("sum(nbcumul0 + nbcumul1) as cumul, " . 
			  "sum(nbexpected0 + nbexpected1) as expected, " . 
			  "strftime('%H:%M',start) as lastminute, " . 
			  "count(id) as nbpid, " . 
			  "sum(value0 + value1) as total " . 
			  "from $tableName " .
			  "group by start order by start desc limit 1");
  }

  private function get_status_mono($tableName)
  {
    return parent::Select("sum(nbcumul) as cumul, " . 
			  "sum(nbexpected) as expected, " . 
			  "strftime('%H:%M',start) as lastminute, " . 
			  "count(id) as nbpid, " . 
			  "sum(value) as total " . 
			  "from $tableName " .
			  "group by start order by start desc limit 1");
  }

  public function getFlowCountingStatus()
  {
    return $this->get_status_bi("FlowCounting_MINUTE");
  }

  public function getZoneCountingStatus()
  {
    return $this->get_status_bi("ZoneCounting_MINUTE");
  }

  public function getZoneNumberingStatus()
  {
    return $this->get_status_mono("ZoneNumbering_MINUTE");
  }

  public function getZoneWaitingTimeStatus()
  {
    return $this->get_status_mono("ZoneWaitingTime_MINUTE");
  }

  public function getWaitingTimePredStatus()
  {
    return parent::Select("count(id) as cumul, " . 
			  "count(id) as expected, " . 
			  "strftime('%H:%M',start) as lastminute, " . 
			  "count(id) as nbpid, " . 
			  "sum(valueSeconds) as total " . 
			  "from WaitingTime_pred_MINUTE " .
			  "group by start order by start desc limit 1");
  }

  public function getNumberingOverflowStatus()
  {
    return parent::Select("count(id) as cumul, " . 
			  "count(id) as expected, " . 
			  "strftime('%H:%M',start) as lastminute, " . 
			  "count(id) as nbpid, " . 
			  "sum(overflow) as total " . 
			  "from Numbering_overflow_MINUTE " .
			  "group by start order by start desc limit 1");
  }

  public function getAllStatus()
  {
    $res = array( "FlowCounting_MINUTE" => $this->getFlowCountingStatus(),
		  "ZoneCounting_MINUTE" => $this->getZoneCountingStatus(),
		  "ZoneNumbering_MINUTE" => $this->getZoneNumberingStatus(),
		  "ZoneWaitingTime_MINUTE" => $this->getZoneWaitingTimeStatus() );

    $module = new WaitingTimeProcessing();
    if($module->haveType("WaitingTime_pred"))
      {
	$res["WaitingTime_pred_MINUTE"] = $this->getWaitingTimePredStatus();
      }

    $module = new NumberingProcessing();
    if($module->haveType("Numbering_overflow"))
      {
	$res["Numbering_overflow_MINUTE"] = $this->getNumberingOverflowStatus();
      }

    return $res;
  }
}

?>