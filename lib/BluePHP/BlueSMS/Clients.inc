<?php

include_once("BluePHP/Utils/ModuleProcessing.inc");

class Clients extends ModuleProcessing
{
  function Clients()
  {
    parent::setParams( array( "name" => "Clients",
			      "keyname" => "id",
			      "sort" => "id" ));
  }  

  function getFunctions()
  {
    $res = array();
    $funcs = parent::Select("* from Functions");
    foreach($funcs as $k => $v)
      {
	$funcs_conf = parent::Select("* from " . $v["id"]);
	foreach($funcs_conf as $l => $w)
	  {
	    $res[] = array( "id" => $v["id"] . "_" . $w["id"],
			    "type" => $v["id"],
			    "type_config" => $w["id"],
			    "type_data" => $w );
	  }
      } 
    return $res;
  }

  function getFunction($params)
  {
    $res = array();
    list($func_type, $func_config) = explode("_", $params["function_id"]);
    $func_data = parent::Select("* from " . $func_type . " where id=\"" . $func_config . "\"");
    return array( "type" => $func_type,
		  "type_config" => $func_config,
		  "type_data" => $func_data[0] );
  }

}

?>

