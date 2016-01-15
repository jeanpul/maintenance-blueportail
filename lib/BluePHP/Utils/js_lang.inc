<?php

$js_var_global = array();

function js_script_begin()
{
  return '<script type="text/javascript">';
}

function js_script_end()
{
  return '</script>';
}

function js_script($data)
{
  return js_script_begin() . "\n" . $data . js_script_end() . "\n";; 
}

function js_decl_global($jsname, $val = null)
{
  global $js_var_global;
  $js_var_global[] = array("jsname" => $jsname,
			   "jsval" => $val);
}

function js_render_var_decl($jsname, $val = null)
{
  $pre_decl = "var $jsname";
  $post_decl = "";
  if(!is_null($val))
    {
      $post_decl .= " = ";
      if(is_float($val) or is_int($val))
	{
	  $post_decl .= $val;
	} 
      else if(is_bool($val))
	{
	  $post_decl .= ($val ? "true" : "false"); 
	}
      else if(is_string($val))
	{
	  $post_decl .= "'" . $val . "'";
	}
      else
	{ // does not support array and object
	  die("BluePHP::js_lang::js_var_decl : error array or object not supported");
	}
    }
  return $pre_decl . $post_decl . ";";
}

function js_render()
{
  global $js_var_global;
  $str = js_script_begin() . "\n";
  foreach($js_var_global as $v)
    {
      $str .= js_render_var_decl($v["jsname"], $v["jsval"]) . "\n";
    }
  $str .= js_script_end() . "\n";
  return $str;
}

function js_funcall($params)
{  
  return $params["function"] . "(" . (isset($params["options"]) ? json_encode($params["options"]) : "") . ");";
}

?>
