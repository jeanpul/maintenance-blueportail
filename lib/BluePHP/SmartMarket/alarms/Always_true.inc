<?php

require_once("BTopLocalServer/BCMServerClient.inc");
require_once("SmartMarket/Sites.inc");

/**
 * Alarme renvoyant toujours vrai, utile pour tester
 * le systeme
 */
class Always_true {

  function process($params)
    {      
      return array( array( "alarm_id" => $params["alarm_id"],
			   "site_id" => $params["site_id"],
			   "site_entity_id" => 0,
			   "cond" => true,
			   "date" => strftime("%F %T") ) );      
    }
}

?>
