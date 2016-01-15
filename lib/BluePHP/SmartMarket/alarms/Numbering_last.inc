<?php

require_once("BTopLocalServer/BCMServerClient.inc");
require_once("SmartMarket/Sites.inc");

/**
 * Alarme basee sur la valeur du numbering
 */
class NumberingAlarm {

  function process($params)
    {      
      return false;
    }

  function process_with_limit($params, $limit)
  {
    $res = array( );
    $module = new Sites();
    $site = $module->getEntry(array( "id" => $params["site_id"] ));
    $bcm = new BCMServerClient($site["server"], $site["blueportail"]);
    $elt = array( "alarm_id" => $params["alarm_id"],
		  "site_id" => $params["site_id"],
		  "site_entity_id" => false,
		  "cond" => false,
		  "date" => false,
		  "value" => false );
    foreach($bcm->getLastMinuteNumberingValues() as $k => $v)
      {
	$elt["site_entity_id"] = $v["id"];
	$elt["date"] = $v["start"];
	$elt["cond"] = $v["value"] > $limit;
	$elt["value"] = $v["value"];
	$res[] = $elt;
      }

    return $res;
  }
}

?>
