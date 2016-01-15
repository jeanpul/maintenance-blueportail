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
 * Compute the numbering by combining multiple
 * zones consedering an area with possible overflow
 */
class Numbering_overflow implements Indicator
{
  protected $clang;  
  protected $zonesParams = array(); // ordered zones ids that reprensent the
  // queue configuration with overflow
  // the first zone match the end of the queue (i.e where people exit)
  // the last zone match the start of the queue (i.e where people enter)
  // each element must be an array containing the zone id "id"
  // the C_max property representing the max number of people threshold "Cmax"
  // the N_qr property [0..1] representing the people ratio which will be considered into the queue
  protected $zonesData = array(); // contain the data values used to compute the
  // final numbering overflow

  function __construct($clang = null)
  {
    $this->clang = $clang;
  }

  public function getProcessingParams($params = "")
  {
    return $this->getIndicatorParams();
  }

  // fab : declaration must be moved to the Indicator interface
  public function setIndicatorParams($p)
  {
    $this->zonesParams = $p;
  }

  public function getIndicatorParams()
  {
    return $this->zonesParams;
  }

  protected function createZonesData($params)
  {
    // retrieve all processing ids for all zones contained in this area
    $module = new NumberingProcessing();
    $npInfo = $module->getEntry( array( "id" => $params["idp"] ) );
    $zonesIdp = $module->getAreaChildNumberingProcessing( array( "id" => $npInfo["zone"] ) );

    // retrieve all numbering values for each zones contained in this area
    // and for the specified period
    $this->zonesData = array();
    foreach($zonesIdp as $k => $v)
      {
	$params["idp"] = $v["idpL"];
	// getNumberingValues must be used instead of getValues
	// to use NumberingType
	$this->zonesData[$v["idL"]] = $module->getNumberingValues($params);	
      }
  }

  protected function getNumberingValue($idp, $idx)
  {
    // compute the w_p = C_p/C_max_p weighted values
    // ,the overflow status and the sum
    $overflow = 0;
    $value = 0;
    $Wp_1 = 1; // previous zone weight
    for($j = 0; $j < count($this->zonesParams); $j++)
      {	    
	$Np = $this->zonesData[$this->zonesParams[$j]["id"]][$idx]["value"];
	$Cp = $Wp_1 * $this->getNumberingQueueRatio($j) * $Np;
	$Cpmax = $this->zonesParams[$j]["Cmax"];
	$Wp = min( $Cp / (float) $Cpmax, 1.0);
	if($Wp == 1.0 && $overflow == $j) // check for overflow
	  { // increase overflow value only if all previous zones are full
	    ++$overflow;
	  }
	$value += $Cp;
	$Wp_1 = $Wp;
      }   
    return array( "id" => $idp,
		  "start" => $this->zonesData[ $this->zonesParams[0]["id"] ][$idx]["start"],
		  "value" => round($value),
		  "overflow" => $overflow );	  
  }

  protected function getNumberingQueueRatio($idx)
  {
    return isset($this->zonesParams[$idx]["Nqr"]) ? $this->zonesParams[$idx]["Nqr"] : 1.0;
  }

  protected function getZonesDataLength()
  {
    return count($this->zonesData[ $this->zonesParams[0]["id"] ]);
  }

  public function process($params)
  {      
    // save idp value
    $idp = $params["idp"];

    if(!isset($params["TimeStr"]))
      {
	$module = new GlobalProperties();
	$params["TimeStr"] = $module->getLastProcessedTimeString();
      }

    // retrieve all numbering values for each zones contained in this area
    // and for the specified period
    $this->createZonesData($params);

    // now run through the time period to compute this indicator
    // use the periods length contained in the first zone
    $numValues = array();

    $idxEnd = $this->getZonesDataLength();
    for($i = 0; $i < $idxEnd; $i++)
      {
	$numValues[] = $this->getNumberingValue($idp, $i);
      }
    return $numValues;
  }

  public function computedByPushAndProcess()
  {
    return false;
  }

  protected function getDBFromDate($date)
  {
    $DBname = substr($date, 0, 4) . substr($date, 4, 2) . substr($date, 6, 2);
    $DBname = BTOP_SERVER_DBPATH . "/days/BCL_" . $DBname . ".db";
    $con = new DBConnect(DATADBTYPE, DATADBHOST, $DBname, 
			 DATADBUSER, DATADBPASSWD);
    $con->connectToDB();
    return $con;
  }

  public function computeInDB($params)
  {
    $values = $this->process($params);
    $con = $this->getDBFromDate($params["TimeStr"]);

    if(!$con->isTableExist("Numbering_overflow_MINUTE"))
      {
	$query = "create table Numbering_overflow_MINUTE(id INTEGER, start TIMESTAMP, " . 
	  "overflow INTEGER, PRIMARY KEY(start, id))";
	$con->query($query);
      }

    foreach($values as $elt)
      {
	$query = "insert or replace into " . "ZoneNumbering_MINUTE " . 
	  "(id, start, value, nbcumul, nbexpected) values " . 
	  "(" . $elt["id"] . ",\"" . $elt["start"] . "\"," . $elt["value"] . 
	  "," . "0,0" . ")";
	$con->query($query);

	$query = "insert or replace into " . "Numbering_overflow_MINUTE " . 
	  "(id, start, overflow) values " . "(" . $elt["id"] . 
	  ",\"" . $elt["start"] . "\"," . $elt["overflow"] . ")";
	$con->query($query);
      }
    $con->closeDB();
    return true;
  }

  public function getValuesInDB($params)    
  {
    $data = array();
    $con = $this->getDBFromDate($params["TimeStr"]);
    $TimeStrDB = convertToDBTimeFormat($params["TimeStr"]);
    // FAB: WARNING ! because we have one table
    // per day we don't use the YYYYMMDD as 
    // selection criterium, this must
    // be reimplemented if the underlying 
    // DB structure change
    $sel = "ZoneNumbering_MINUTE.id as id, ZoneNumbering_MINUTE.start as start, " . 
      "value, overflow";
    $cond = "ZoneNumbering_MINUTE.start=Numbering_overflow_MINUTE.start and " .
      "ZoneNumbering_MINUTE.id=Numbering_overflow_MINUTE.id and " . 
      "ZoneNumbering_MINUTE.id=" . $params["idp"];
    if($params["CompPeriod"] == "minute")
      {
	$cond .= " and ZoneNumbering_MINUTE.start=\"$TimeStrDB\"";
      }
    $order = "ZoneNumbering_MINUTE.start asc";
    $query = "select $sel from ZoneNumbering_MINUTE, Numbering_overflow_MINUTE " . 
      "where $cond order by $order";
    $result = $con->query($query);
    $data = $con->getAssociativeArray($result);
    $result->freeResult();
    $con->closeDB();
    return $data;
  }
}


?>