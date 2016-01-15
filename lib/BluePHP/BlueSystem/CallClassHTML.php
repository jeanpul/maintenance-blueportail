<?php

error_reporting(E_ALL);
ini_set('error_reporting', E_ALL);
ini_set('display_errors', true);

$params = array( "__class" => null,
		 "__id" => "",
		 "__function" => "render" );

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

$classPath = strtr($params["__class"], ".", "/");
$className = basename($classPath);

include_once("BluePHP/" . $classPath . ".inc");

$id = $params["__id"];

$widget = new $className($id, $params);
call_user_func(array($widget, $params["__function"]));

?>