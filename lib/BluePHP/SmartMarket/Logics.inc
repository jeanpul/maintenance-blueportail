<?php

include_once("BluePHP/ModuleProcessing.inc");

class Logics extends ModuleProcessing
{
  function Logics()
  {
    parent::setParams( array( "name" => "Logics",
			      "keyname" => "id",
			      "sort" => "id" ));
  }  

  function getConfigs()
  {
    $res = array();
    $funcs = parent::Select("* from Configs");
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

  function getConfig($params)
  {
    $res = array();
    list($func_type, $func_config) = explode("_", $params["config_id"]);
    $func_data = parent::Select("* from " . $func_type . " where id=\"" . $func_config . "\"");
    return array( "type" => $func_type,
		  "type_config" => $func_config,
		  "type_data" => $func_data[0] );
  }

  function getConfigClass($config)
  {
    $basename = $config["type"];
    $file = "configs/${basename}.inc";
    if(!file_exists($file))
      {
	return false;
      }
    include_once($file);
    return new $basename();
  }

}

?>

