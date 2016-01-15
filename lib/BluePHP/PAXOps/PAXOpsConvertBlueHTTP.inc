<?php

include_once("BluePHP/Utils/DateOps.inc");
include_once("PAXOpsParser.inc");
include_once("PAXOps.inc");

/**
 * From the XML document parsed and from
 * the database containing the PAXOps => BCM information
 * create an array the BlueHTTP URLS
 */
function PAXOpsConvertBlueHTTP($parser, $module)
{
  $urls = array();
  $entries = $module->getEntries();
  $timeRange = $parser->getTime();
  foreach($parser->getSensorList() as $POSensor)
    {
      foreach($entries as $BCMSensor)
	{
	  if($POSensor["ID"] == $BCMSensor["sensorId"])
	    {
	      $baseURL = "http://" . $BCMSensor["bluehttp_server"] . ":" . $BCMSensor["bluehttp_port"] . "/" .
		$BCMSensor["bluehttp_script"] . "?" . "client=" . $BCMSensor["clientId"] . "&type=147&channel=0" . 
		"&timeStart=" . convertCommonTimeString($timeRange["STARTTIME"]) . 
		"&timeEnd=" . convertCommonTimeString($timeRange["ENDTIME"]) . 
		"&counter=0&value=" . $POSensor["VALUE"];
	      $urls[] = $baseURL;
	    }
	}
    }
  return $urls;
}

?>
