<?php

// openflashchart 2 utilities

function createOFCObject($params, $chart = null)
{
  $params = array_merge( array ( "id" => "default", 
				 "width" => "100%", "height" => "100%",
				 "getDataFunc" => "get_data" ),
			 $params );
  $str = '<script type="text/javascript">' . "\n";
  $str .= "swfobject.embedSWF('/OFC/open-flash-chart.swf', '" . $params["id"] . 
    "','" . $params["width"] . "','" . $params["height"] . 
    "','9.0.0', 'expressInstall.swf', { 'get-data' : '" . 
    $params["getDataFunc"] . "' });\n";

  $str .= "function " . $params["getDataFunc"] . "()\n" . 
    "{\n return JSON.stringify(" . $params["id"] . ");\n}\n";

  if($chart)
    {
      $str .= "var " . $params["id"] . "=" . $chart->toPrettyString() . ";\n";
    }

  $str .= "</script>\n";
  return $str;
}

?>