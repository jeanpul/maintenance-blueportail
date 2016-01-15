<?php

error_reporting(E_ALL);
ini_set('error_reporting', E_ALL);
ini_set('display_errors', true);

include_once("BluePHP/BlueProjectPHP/Config.inc");

$params = array( "__class" => null );

if(PHP_SAPI == 'cli')
  {
    $params = array_merge($params, $argv);
  }
else
  {
    $params = array_merge($params, $_GET);
    foreach($params as $k => $v)
      {
	$params[$k] = urldecode($v);
      }
  }

include_once("BluePHP/BlueProjectPHP/" . $params["__class"] . ".inc");

$module = new $params["__class"]();
$res = call_user_func(array($module, $params["__function"]), $params);

header('Cache-Control: no-cache, must-revalidate');
header('Content-type: application/json');
echo json_encode($res);

?>