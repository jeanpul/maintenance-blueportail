<?php

function getHeader($title, $link2 = "", $link2text = "insert a new entry")
{
  $str = "<html>\n";
  $str .= "<head>\n";
  $str .= '<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />' . "\n";
  $str .= "<title>" . $title . "</title>\n";
  $str .= "</head>\n"
    . "<body>\n" . "<h1>" . $title . "</h1>\n"
    . '<a href="http://pong/">Return to main menu</a>';

    if($link2)
	{
	    $str .= ' or <a href="' . $link2 . '">' . $link2text . '.</a>' . "\n";
	}
  $str .= "<hr>\n";
  return $str;	
}

function getFooter()
{
  $str = "</body>\n</html>\n";
  return $str;
}

function getHeaderInsertPage($title)
{
  $str = '<!DOCTYPE html ' . "\n" .
    'PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" ' . "\n" . 
    '"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">' . "\n";
  $str .= "<html>\n";
  $str .= "<head>\n";
  $str .= '<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1"></meta>' . "\n";
  $str .= "<title>Insert a new Entry into " . $title . "</title>\n";
  $str .= "</head>\n";
  $str .= "<body>\n";
  return $str;	
}

function getFooterInsertPage($mainPage, $mainPageTitle, $insertLink)
{
  $str = "<a href=\"$mainPage\">Return to $mainPageTitle</a>";
  $str .= " or ";
  $str .= "<a href=\"$insertLink\">insert a new entry</a>\n.";
  $str .= " <body>\n<html>\n";
  return $str;
}

function getHeaderInputPage($title)
{
  $str = '<!DOCTYPE html ' . "\n" .
    'PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" ' . "\n" . 
    '"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">' . "\n";
  $str .= "<html>\n";
  $str .= "<head>\n";
  $str .= '<meta http-equiv="content-type" content="text/html; charset=ISO-8859-1">' . "\n";
  $str .= "<title>". $title ."</title>\n";
  $str .= "  <script language=\"JavaScript\" SRC=\"libraries/AnchorPosition.js\"></script>\n";
  $str .= "  <script language=\"JavaScript\" SRC=\"libraries/PopupWindow.js\"></script>\n";
  $str .= "  <script language=\"JavaScript\" SRC=\"libraries/date.js\"></script>\n";
  $str .= "  <script language=\"JavaScript\" SRC=\"libraries/CalendarPopup.js\"></script>\n";
  $str .= "  <script language=\"JavaScript\" SRC=\"libraries/BlueAjax.js\"></script>\n"; 
  $str .= "</head>\n";
  $str .= "<body>\n";
  return $str;
}

function getInputText($data, $params)
{
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

  $str = "<input type=\"text\" name=\"" . $params["name"] . "\" value=\"" . utf8_encode($data) . 
    "\" $readonly $extra/>\n";

  return $str;
}

function getInput($params)
{
  $params = array_merge(array( "type" => "text", "name" => "", "id" => "",
			       "value" => "", "readonly" => false, "extra" => ""),
			$params);
  return "<input type=\"" . $params["type"] . "\" id=\"" . $params["id"] . "\" name=\"" . $params["name"] . "\" value=\"" .
    utf8_encode($params["value"]) . "\" " . ($params["readonly"] ? "readonly=\"readonly\"" : "") . 
    " " . $params["extra"] . "/>\n";
}

function getForm($data, $params)
{
  $params = array_merge(array( "method" => "get", "action" => "",
			       "name" => "default" ),
			$params);
  return "<form method=\"" . $params["method"] . "\" action=\"" . $params["action"] . "\"" . 
    " name=\"" . $params["name"] . "\">\n" . $data . "</form>\n";
}

function getCloseButton()
{
  return '<form method="post"><input type="button" value="Close Window"onclick="window.close()">';
}

function getInputTextArea($data, $params)
{
  $extra = "";
  if($params["extra"])
    {
      $extra = $params["extra"];
    }

  return "<textarea name=\"" . $params["name"] . "\" rows=\"" . $params["rows"] . 
    "\" cols=\"" . $params["cols"] . "\" $extra>" . utf8_encode($data) . "</textarea>\n";
}

function getInputDate($inputName, $defaultValue = "")
{
  $str = '<script language="JavaScript" ID="js1">' . "\n";
  $str .= 'var cal1 = new CalendarPopup();' . "\n";
  $str .= '</script>' . "\n";
  //  $str .= '<script language="JavaScript">writeSource("js1");</script>' . "\n";
  $str .= '<input type="text" name="' . $inputName . '" value="' . $defaultValue . '" size="25" />' . "\n";
  $str .= '<a href="#" onClick="cal1.select(document.forms[0].' .
    $inputName . ",'anchor1','yyyyMMdd'); " . 'return false;" title="cal1.select(document.forms[0].' .
    $inputName . ",'anchor1','MM/dd/yyyy'); " . 'return false;" name="anchor1" id="anchor1">select</a>';
  return $str;
}

function getHiddenInput($inputName, $value)
{
  return "<input type='hidden' name='$inputName' value='$value'/>\n";
}

function getInputSelect($inputName, $connexion, $tableName, $defaultValue = "")
{
  $str = '<input id="' . $inputName . '" type="text" name="' . $inputName . '" value="' . $defaultValue . '" />' . "\n";
  $str .= getSelect($inputName, $connexion, $tableName, $defaultValue);
 return $str;
}

function getSelect($inputName, $connexion, $tableName, $defaultValue = "")
{
  $str = '<select name="' . $inputName . '" onChange="document.getElementById(' . "'" . $inputName . "'" .
    ').value=document.getElementById(' . "'" . $inputName . "'" . ').value">' . "\n";
  
  $query = "SELECT DISTINCT " . $inputName . " FROM " . $tableName .
    " WHERE " . $inputName . ' != "" ORDER BY ' . $inputName;
  $result = Query($query, $connexion);

  while($row = NextAssoc($result))
    {
      if($row["$inputName"] == $defaultValue)
	{
	  $str .= '<option value="' . $row["$inputName"] . '" selected>' . $row["$inputName"] . '</option>' . "\n";
	}
      else
	{
	  $str .= '<option value="' . $row["$inputName"] . '" >' . $row["$inputName"] . '</option>' . "\n";
	}
    }
  $str .= "</select>\n";
 return $str;  
}

function getSelectTemplate($data, $params)
{
  $columns = explode(',', $params["value"]);

  $extra = "";
  if($params["extra"])
    {
      $extra = $params["extra"];
    }

  $str = "<select name=\"" . $params["name"] . "\" $extra>\n";
  foreach($data as $k => $v)
    {
      $selected = "";
      if($params["default"] == $v[$params["key"]])
	{
	  $selected .= " selected";
	}
      $str .= "<option value=\"" . $v[$params["key"]] . "\" $selected>";
      foreach($columns as $c)
	{
	  $str .= utf8_encode($v[$c]) . " ";
	}
      $str .= "</option>\n";
    }
  $str .= "</select>\n";
  return $str;
}

function getOption($value, $descr, $selected = false)
{
  if($selected)
    {
      return "<option value=\"$value\" selected>$descr</option>\n";
    }
  return "<option value=\"$value\">$descr</option>\n";
}

function getSelectEnum($inputName, $enum, $default = "")
{
  $str = "<select name=\"$inputName\">\n";
  foreach($enum as $e)
    {
      if($default == $e)
	{
	  $str .= "<option value=\"$e\" selected>$e</option>\n";
	}
      else
	{
	  $str .= "<option value=\"$e\" >$e</option>\n";
	}
    }
  $str .= "</select>\n";
  return $str;
}

function getButtonLink($href, $text)
{
  $str = '<form action="' . $href . '" method="POST" >' . "\n";
  $str .= '<button type="submit">' . "\n";
  $str .= "<strong>" . htmlentities($text) . "</strong>\n" . "</button>\n" . "</form>\n"; 

  return $str;
}

function get2CellsRow($cell1, $cell2)
{
  return "<tr><td>" . $cell1 . "</td><td>" . $cell2 . "</td></tr>\n";
}

function getFileUpload($filename, $inputname, $inputvalue, $action = "upload.php")
{
  $str = "";
  $str .= '<form method="post" enctype="multipart/form-data" action="upload.php">' . "\n";
  $str .= '<input type="hidden" name="filename" value="' . $filename . "\">\n";
  $str .= '<input type="hidden" name="' . $inputname . '" value="' . $inputvalue . "\">\n";
  $str .= "<table>\n"; 
  $str .= get2CellsRow('<input type="file" name="file" size="48" maxlength="128" >',
		       '<input type="submit" name="upload" value="file">');
  $str .= get2CellsRow('<b>Utiliser le scanner de l\'imprimante<b>',
		       '<input type="submit" name="upload" value="scan">');
  $str .= "</table>\n";
  $str .= "</form>\n";
  return $str;
}

?>
