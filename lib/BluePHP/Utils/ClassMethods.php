<?php

error_reporting(E_ALL);
ini_set('error_reporting', E_ALL);
ini_set('display_errors', true);

$params = array( "__class" => null,
		 "__id" => "",
		 "__function" => null );

if(PHP_SAPI == 'cli')
  {
    $output = array();
    parse_str($argv[1], $output);
    $params = array_merge($params, $output);
  }
else
  {
    $params = array_merge($params, $_GET);
  }

$hrefBase = $_SERVER["PHP_SELF"] . "?__class=" . $params["__class"];
$classPath = strtr($params["__class"], ".", "/");
$className = basename($classPath);

include_once("BluePHP/" . $classPath . ".inc");

$reflector = new ReflectionClass($className);            

$res = array( "class" => $className,
	      "methods" => array() );
    
$cons = $reflector->getConstructor();
$consName = $cons->getName();

$methods = $reflector->getMethods( ReflectionMethod::IS_PUBLIC );
foreach($methods as $m)
  {
    $methodName = $m->getName();
    if($methodName != $consName)
      {
	$res["methods"][] = $methodName;
      }
  }

header('Cache-Control: no-cache, must-revalidate');
header('Content-type: application/json');
echo json_encode($res); 

?>

