<?php

/**
 * A set of functions to easy manipulate Jquery
 * and Javascript rendering
 * argsReplace could be used in order to enable js function not supported
 * by json_encode
 * example $args = array( "toto" => 10, "titi" => "%func%"), $argsReplace = array( "%func%" => "function() {}" )
 * the json at end will be { "toto": 10, "titi" : function() {} } avoiding the quotes arount the function def.
 */
function jquery_id_call($id, $func, $args = null, $argsReplace = null)
{
  $params = "";
  if(!is_null($args))
    {
      $params = json_encode($args);
    }
  if(!is_null($argsReplace))
    {
      foreach($argsReplace as $k => $v)
	{
	  $params = str_replace("\"$k\"", $v, $params);
	}
    }
  return "$('#$id').$func($params);";
}

?>