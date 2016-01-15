<?php

function retrieveArgs($params = array())
{
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
  return $params;
}

function createInput($params)
{
  $params = array_merge(array( "type" => "text", 
			       "name" => "", 
			       "id" => "",
			       "value" => "", 
			       "readonly" => false, 
			       "extra" => "",
			       "checked" => false,
			       "disabled" => false ),
			$params);
  return "<input type=\"" . $params["type"] . "\"" . ($params["id"] != "" ? " id=\"" . $params["id"] . "\" " : "") .    
    "name=\"" . $params["name"] . "\" value=\"" .
    $params["value"] . "\" " . ($params["readonly"] ? "readonly=\"readonly\"" : "") . 
    " " . (($params["type"] == "radio" and $params["checked"] == true) ? "checked=\"checked\"" : "") . 
    " " . (($params["disabled"] == true) ? "disabled=\"true\"" : "") . 
    " " . $params["extra"] . "/>\n";
}

function createSubmit($data)
{
  return createInput(array("type" => "submit", "value" => $data, "name" => "submit" ));
}

function createCheckBox($params, $checked = false)
{
  $extraSaved = isset($params["extra"]) ? $params["extra"] : "";
  return createInput(array_merge($params,
				 array("type" => "checkbox",
				       "value" => "true",
				       "extra" => $checked ? $extraSaved . 
				       " checked=\"checked\"" : $extraSaved)));
}

function createTrueFalseSwitch($params)
{
  return createSelect( array( array( "v" => "false", "k" => 0 ),
			      array( "v" => "true", "k" => 1 ) ),
		       array_merge( $params, 
				    array( "key" => "k",
					   "value" => "v" ) ) );
}

function createInputHidden($name, $value = "")
{
  return createInput(array( "type" => "hidden", "name" => $name, "value" => $value));
}

function createInputPassword($name, $value = "")
{
  return createInput(array( "type" => "password", "name" => $name, "value" => $value));
}

function createInputText($data, $params)
{
  $params = array_merge( array( "readonly" => false,
				"extra" => false,
				"size" => null,
				"id" => null,
				"maxlength" => null,
				"OnNullValue" => null,
				"title" => "" ),
			 $params );

  $readonly = "";
  if($params["readonly"])
    {
      $readonly = "Readonly";
    }
  $extra = "";
  if($params["extra"])
    {
      $extra = $params["extra"];
    }

  $str = "<input title=\"" . $params["title"] . "\" type=\"text\" name=\"" . $params["name"] . "\" value=\"" . 
    (is_null($data) ? $params["OnNullValue"] : $data) . 
    "\"" . " " . (!is_null($params["size"]) ? "size=" . $params["size"] : "") .
    " " . (!is_null($params["maxlength"]) ? "maxlength=" . $params["maxlength"] : "") .    
    " " . (!is_null($params["id"]) ? "id=\"" . $params["id"] . "\" " : " ") . "$readonly $extra/>\n";
  return $str;
}

function createInputCurrency($data, $params)
{
  $params = array_merge( array( "id" => "inputCurrency", 
				"prefix" => "",
				"suffix" => "",
				"allowNegative" => false,
				"thousandsSeparator" => "" ), $params );
  $str = createInputText($data, $params);
//  $options = json_encode( array( "prefix" => $params["prefix"],
//				 "suffix" => $params["suffix"],
//				 "allowNegative" => $params["allowNegative"],
//				 "thousandsSeparator" => $params["thousandsSeparator"],
//				 "centsSeparator" => ',' ));
//  $str .= '<script type="text/javascript">' . "\n";
//  $str .= '$("#' . $params["id"] . '").priceFormat(' . $options . ');' . "\n";
//  $str .= "</script>\n";
  return $str;
}

function createInputTextArea($data, $params)
{
  $params = array_merge( array( "extra" => null ), $params );

  $extra = "";
  if($params["extra"])
    {
      $extra = $params["extra"];
    }

  return "<textarea name=\"" . $params["name"] . "\" rows=\"" . $params["rows"] . 
    "\" cols=\"" . $params["cols"] . "\" $extra>" . $data . "</textarea>\n";
}

function createSelect($data, $params)
{
  $params = array_merge( array( "id" => false,
				"readonly" => false,
				"extra" => false,
				"default" => "",
				"sep" => " ",
				"selectCb" => false,
				"disabled" => false ),
			 $params );

  $isSimpleArrayData = !isset($params["key"]) or $params["key"] == false;
  $columns = $isSimpleArrayData ? false : explode(',', $params["value"]);

  $extra = "";
  if($params["extra"])
    {
      $extra = $params["extra"];
    }

  $str = "<select ". ($params["id"] ? "id=\"" . $params["id"] . "\" " : "") . "name=\"" . $params["name"] . "\" " . 
    ($params["disabled"] ? "disabled=\"disabled\" " : "") . 
    "$extra>\n";
  foreach($data as $k => $v)
    {
      $selected = "";
      $value = $isSimpleArrayData ? $v : $v[$params["key"]];
      if(!$params["selectCb"])
	{
	  if((!is_array($params["default"]) and $params["default"] == $value) or
	     (is_array($params["default"]) and in_array($value, $params["default"])))
	    {
	      $selected .= " selected";
	    }
	}
      else if($params["selectCb"]($value, $params))
	{
	  $selected .= " selected";
	}
      $str .= "<option value=\"$value\" $selected>";
      if($isSimpleArrayData)
	{
	  $str .= $value;
	}
      else
	{
	  $firstCol = true;
	  foreach($columns as $c)
	    {
	      if($firstCol)
		{
		  $firstCol = false;
		}
	      else
		{
		  $str .= $params["sep"];
		}
	      $str .= $v[$c];
	    }
	}
      $str .= "</option>\n";
    }
  $str .= "</select>\n";
  return $str;
}

function createTableBegin($params = array())
{
  return "<table>\n";
}

function createTableEnd($params = array())
{
  return "</table>\n";
}

function createTableRow($data, $params = array())
{
  $params = array_merge( array( "type" => "td",
				"tr" => "tr" ),
			 $params );
  $strPre = "";
  $strPost = "";
  if($params["tr"]) 
    { 
      $strPre = "<" . $params["tr"] . ">\n"; 
      $strPost = "</" . $params["tr"] . ">\n"; 
    }
  $str = $strPre;
  foreach($data as $k => $v)
    {
      $str .= "<" . $params["type"] . ">" . $v . "</" . $params["type"] . ">";
    }
  $str .= $strPost;
  return $str;
}

function createList($data, $params = array())
{
  $params = array_merge( array( "type" => "li" ),
			 $params );
  $str = "<ul>";
  foreach($data as $k => $v)
    {
      $str .= "<" . $params["type"] . ">" . $v . "</" . $params["type"] . ">";
    }
  $str .= "</ul>\n";
  return $str;
}

function createTableHeaderRow($data, $params = array())
{
  return createTableRow($data, array_merge($params, array( "type" => "th" )));
}

function createAction($f, $params, $action = "onChange")
{
  $fcall = $action . "='$f(";
  $firstComa = true;
  foreach($params as $k => $v)
    {
      if($firstComa)
	{
	  $firstComa = false;
	}
      else
	{
	  $fcall .= ",";
	}
      $fcall .= "\"$v\"";
    }
  $fcall .= ")'";
  return $fcall;
}

function createInputDate($data, $params)
{
  $params = array_merge( array( "name" => "date",
				"format" => "yyyy-mm-dd",
				"title" => "Date" ), $params );

  $jsvar = $params["name"] . "_js";
  $idanchor = $params["name"] . "_anchor";

  $str = '<script language="JavaScript">' . "\n";
  $str .= 'var ' . $jsvar . ' = new CalendarPopup();' . "\n";
  $str .= '</script>' . "\n";
  $str .= '<input type="text" name="' . $params["name"] . '" value="' . $data . '" size="12" />' . "\n";
  $str .= '<a href="#" onClick="' . $jsvar . '.select(document.forms[0].' .
    $params["name"] . ",'" . $idanchor . "','" . $params["format"] . "'); " . 
    'return false;" " id="' . $idanchor . '" name="calendar">'. $params["title"] . '</a>';
  return $str;
}

function createInputDatePicker($data, $params)
{
  $params = array_merge( array( "id" => "inputDate", 
				"name" => "date",
				"format" => "yy-mm-dd",
				"title" => "Date" ), $params );

  $str = createInputText($data, $params);
  $str .= '<script type="text/javascript">' . "\n";
  $str .= '$("#' . $params["id"] . '").datepicker({ showWeek: true, dateFormat: "' . 
    $params["format"] . '"});' . "\n";
  $str .= "</script>\n";
  return $str;
}

function createForm($data, $params = array())
{
  $params = array_merge(array( "method" => "get", "action" => "",
			       "name" => "default", "id" => "Form" ),
			$params);
  $str = "<form id=\"" . $params["id"] . "\" method=\"" . $params["method"] . "\" action=\"" . $params["action"] . "\"" . 
    " name=\"" . $params["name"] . "\">\n";
  if(is_array($data))
    {
      foreach($data as $elt)
	{
	  $str .= $elt;
	}
    }
  else
    {
      $str .= $data;
    }
  $str .= "</form>\n";
  return $str;
}

function createLink($params)
{
  $params = array_merge( array( "url" => "#", 
				"value" => "none",
				"nofollow" => false ),
			 $params);

  return "<a " . ($params["nofollow"] ? "rel='nofollow' target='_blank' " : "")  . "href=\"" . 
    $params["url"] . "\">" . $params["value"] . "</a>";
}

function createApplyButton($name)
{
  return '<button type="submit" name="action" value="' . $name . '">' . $name . '</button>';
}

function parseSimplePairValues($elts, $params = array())
{
  $params = array_merge( array( "sep" => "=", "trim" => " \"" ), $params );
  $res = array();
  foreach($elts as $elt)
    {
      $e = explode($params["sep"], $elt, 2);
      if(is_array($e) and count($e))
	{
	  $res[ trim($e[0], $params["trim"]) ] = trim($e[1], $params["trim"]);
	}
    }
  return $res;
}

/**
 * Returns the url with extra input parameters used
 * by BluePortail if activated
 */
function getBluePortailURL($url)
{
  global $clientBluePortail;

  if($clientBluePortail)
    {
      list($base, $parameters) = explode("?", $url, 2);
      $base .= "?clientBluePortail=" . $clientBluePortail;
      if(count($parameters))
	{
	  $base .= "&" . $parameters;
	}
      return $base;
    }
  return $url;
}

/**
 * Returns some extra inputs parameters used
 * by BluePortail if activated
 */
function getBluePortailInputs()
{
  global $clientBluePortail;

  if($clientBluePortail)
    {
      return "<input type=\"hidden\" name=\"clientBluePortail\" value=\"$clientBluePortail\" />\n";
    }
  return "";
}

?>
