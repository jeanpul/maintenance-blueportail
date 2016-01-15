<?php

error_reporting(E_ALL);
ini_set('error_reporting', E_ALL);
ini_set('display_errors', true);

include_once("BluePHP/BlueProjectPHP/Config.inc");

$params = array( "__class" => null,
		 "__id" => null );

if(PHP_SAPI == 'cli')
  {
    $params = array_merge($params, array( "__class" => $argv[1] ));
  }
else
  {
    $params = array_merge($params, $_GET);
  }

include_once("BluePHP/BlueProjectPHP/" . $params["__class"] . ".inc");

$id = !is_null($params["__id"]) ? $params["__id"] : $params["__class"];

$widget = new $params["__class"]($id, $params);
$widget->render();

?>
