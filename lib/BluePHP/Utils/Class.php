<?php

error_reporting(E_ALL);
ini_set('error_reporting', E_ALL);
ini_set('display_errors', true);

$params = array( "__class" => null,
		 "__id" => "",
		 "__function" => null );

$funcArgs = array();

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

$hrefBase = $_SERVER["PHP_SELF"];

$classParts = explode(".", $params["__class"]);
$classPartsLink = "";
$firstElem = true;
$classConcat = "";
foreach($classParts as $part)
  {
    if($firstElem)
      {
	$firstElem = false;
      }
    else
      {
	$classPartsLink .= "<span class='navSep'>.</span>";
	$classConcat .= ".";
      }
    $classConcat .= $part;
    $classPartsLink .= "<a href='$hrefBase?__class=$classConcat'>$part</a>";
  }

$hrefBase = $_SERVER["PHP_SELF"] . "?__class=" . $params["__class"];
$link = "<a href='$hrefBase'>$classPartsLink</a>";
$classPath = strtr($params["__class"], ".", "/");
$className = basename($classPath);

if(is_dir($_SERVER["DOCUMENT_ROOT"] . "/BluePHP/" . $classPath))
  {
    $str = "";

    $str .= "<div id='$className' class='dir'>" . "\n" . 
      "<span class='dirName'>$link</span>" . "\n";

    $str .= "<ul class='methodsList'>" . "\n";
    foreach(glob($_SERVER["DOCUMENT_ROOT"] . "/BluePHP/" . $classPath . "/*.inc") as $filename)
      {
	$filename = basename($filename, ".inc");
	$link = "<a href='$hrefBase.$filename'>$filename</a>";
	$str .= "<li id='$filename' class='file'>" . "\n" . 
	  "<span class='fileName'>$link</span>" . "\n";
	$str .= "</li>" . "\n";	
      }
    $str .= "</ul>" . "\n";
    $str .= "</div>" . "\n";
  }
else
  {
    include_once("BluePHP/" . $classPath . ".inc");
    
    $str = "";
    
    $reflector = new ReflectionClass($className);            
    
    $str .= "<div id='$className' class='class'>" . "\n" . 
      "<span class='className'>$link</span>" . "\n";
    
    $cons = $reflector->getConstructor();
    $consName = $cons->getName();
    
    $str .= "<div id='$consName' class='method cons'>" . "\n" .
      "<span class='methodName'>$consName</span>" . "\n";  
    $str .= "</div>" . "\n";
    
    // public methods only
    $str .= "<ul class='methodsList'>" . "\n";
    
    $methods = $reflector->getMethods( ReflectionMethod::IS_PUBLIC );
    foreach($methods as $m)
      {
	$methodName = $m->getName();
	if($methodName != $consName)
	  {
	    $methodNameLabel = $methodName . " (";
	    $firstParam = true;
	    foreach($m->getParameters() as $v)
	      {
		if(isset($params[$v->getName()]))
		  {
		    $funcArgs[] = $params[$v->getName()];
		  }
		if($v->isDefaultValueAvailable())
		  {
		    $str .= json_encode($v->getDefaultValue());
		  }
		if($firstParam)
		  {
		    $firstParam = false;
		  }
		else
		  {
		    $methodNameLabel .= ",";
		  }
		$methodNameLabel .= $v->getName();
	      }
	    $methodNameLabel .= ")";
	    $link = "<a href='$hrefBase&__function=$methodName'>$methodNameLabel</a>";
	    $str .= "<li id='$methodName' class='method'>" . "\n" . 
	      "<span class='methodName'>$link</span>" . "\n";
	    $str .= "</li>" . "\n";
	  }       
      }
    
    $str .= "</ul>" . "\n";
    
    if($params["__function"] != null)
      {
	$obj = new $className();
	if(count($funcArgs))
	  {
	    $res = call_user_func_array(array($obj, $params["__function"]), $funcArgs);
	  }
	else
	  {
	    $res = call_user_func(array($obj, $params["__function"]));
	  }
	$str .= json_encode($res);
      }
    
    $str .=" </div>" . "\n";
  }


if(PHP_SAPI == 'cli')
  {
    echo json_encode($res) . "\n";
  }
else
  {
?>

<html>
<head>
<script src="/JQUERY/jquery-latest.js" type="text/javascript"></script>
</head>
<body>
<?php echo $str . "\n"; ?></body>
<script type="text/javascript">
</script>

<div id="resCall">
</div>

</html>

<?php
      }
?>