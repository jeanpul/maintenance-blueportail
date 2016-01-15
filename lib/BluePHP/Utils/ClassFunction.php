<?php

error_reporting(E_ALL);
ini_set('error_reporting', E_ALL);
ini_set('display_errors', true);

set_include_path(get_include_path() . PATH_SEPARATOR . '/var/www/BluePHP');

$params = array( "__class" => null,
		 "__construct" => null,
		 "__call" => array( "__function" => "render",
				    "__params" => null ),
		 "__output" => "HTML" );

if(PHP_SAPI == 'cli')
  {
    $buffer = "";
    while(!feof(STDIN))
      {
	$buffer .= fgets(STDIN);
      }
    $data = json_decode($buffer, true);
    $params = array_merge($params, $data);
  }
else
  {
    $params = array_merge($params, $_POST);
  }

$classPath = strtr($params["__class"], ".", "/");
$className = basename($classPath);

include_once($classPath . ".inc");

$refClass = new ReflectionClass($className);

$module = null;

if($params["__construct"] and
   is_array($params["__construct"]))
  {
    $module = $refClass->newInstanceArgs($params["__construct"]);
  }
else
  {
    $module = $refClass->newInstance();
  }

$refMethod = $refClass->getMethod($params["__call"]["__function"]);

$res = null;
if(isset($params["__call"]["__params"]) and
   is_array($params["__call"]["__params"]))
  {
    $res = $refMethod->invokeArgs($module, $params["__call"]["__params"]);
  }
else
  {   
    $res = $refMethod->invoke($module);
  }

if($params["__output"] == "JSON_dataTables")
  {
    $res = array( "sEcho" => 0,
		  "iTotalRecords" => count($res),
		  "iTotalDisplayRecords" => count($res),
		  "aaData" => $res );
  }

if(!headers_sent() and PHP_SAPI != 'cli')
  {
    header('Cache-Control: no-cache, must-revalidate');
    header('Content-type: application/json');
  }
echo json_encode($res); 

?>

