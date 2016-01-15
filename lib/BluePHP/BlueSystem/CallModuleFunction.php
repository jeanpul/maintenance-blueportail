<?php

error_reporting(E_ALL);
ini_set('error_reporting', E_ALL);
ini_set('display_errors', true);

$params = array( "__class" => null,
		 "__module" => "BlueSystem",
		 "__output" => "JSON",
		 "__function" => "render",
		 "__id" => null,
		 "__sep" => ';',
		 "sEcho" => 0 );

if(PHP_SAPI == 'cli')
  {
    $output = array();
    parse_str($argv[1], $output);
    $params = array_merge($params, $output);
  }
else
  {
    $params = array_merge($params, $_GET);
    foreach($params as $k => $v)
      {
	$params[$k] = urldecode($v);
      }
  }

include_once("BluePHP/" . $params["__module"] . "/" . $params["__class"] . ".inc");

$module = new $params["__class"]();

$res = call_user_func(array($module, $params["__function"]), $params);

if($params["__output"] == "JSON_dataTables")
  {
    $res = array( "sEcho" => $params["sEcho"],
		  "iTotalRecords" => count($res),
		  "iTotalDisplayRecords" => count($res),
		  "aaData" => $res );
  }

if($params["__output"] == "JSON" or $params["__output"] == "JSON_dataTables")
  {
    header('Cache-Control: no-cache, must-revalidate');
    header('Content-type: application/json');
    echo json_encode($res); 
  }
else if($params["__output"] == "CSV")
  {
    foreach($res as $v)
      {
	if(is_array($v))
	  {
	    echo implode($params["__sep"], array_values($v)) . "\n";
	  }
      }
  }
else if($params["__output"] == "HTML")
  {
    echo $res;
  }
else if($params["__output"] == "DEBUG")
  {
    print_r($res);
  }

?>