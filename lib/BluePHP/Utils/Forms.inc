<?php

/* 
 * Forms.inc
 *
 */

//include_once("Config.inc");
include_once("DBConnect.inc");
include_once("FormPage.inc");
include_once("Languages.inc");

/**
 * Images list
 */
$Images = array ("FileAttente" => "/BluePHP/Graphics/FileAttente.gif",
		 "Comptage" => "/BluePHP/Graphics/Comptage.gif",
		 "BEV_LOGO_MR" => "/BluePHP/Graphics/BEV_LOGO_MR.gif",
		 "BEV_LOGO_LR" => "/BluePHP/Graphics/BEV_LOGO_LR.gif",
		 "BEVWithDegrade" => "/BluePHP/Graphics/BEVWithDegrade.gif",
		 "CameraView" =>  "/BluePHP/Graphics/FileAttente.gif");

/**
 * Displays a list that contains 
 * the menu entries. Each menu entry
 * is a li item from en ul lisdt. Accesses rights are
 * checked for each entry.
 */
function getMenu($args = "")
{
  global $PagesInMenu;
  global $PagesAccess;

  // GRAPHIC STUFF
  $menu =  "<div id=\"menu\"><h3 id=\"titleSec\">" . myhtmlentities(_('Functionalities')) . "</h3><ul>\n";
  foreach($PagesInMenu as $id => $link)
    {
      $menu .= "<li>" . "<a href=\"" . $link . $args . "\">" . myhtmlentities(_($id)) . "</a></li>\n";
    }
  $menu .= "</ul></div>\n";
  // GRAPHIC STUFF

  return $menu;
}

/**
 * Displays the 3 navigation buttons
 * to go through the menu entries.
 * One of the button is used to access
 * directly to the main page (menu page).
 * Accesses rights are checked.
 * $whereami parameter is the index of the current page
 * into the $Pages list.
 */
function formNavBar($whereami)
{
  global $Pages;
  global $PagesAccess;

  $navbar = "<table summary=\"Navigation bar\" class=\"navbar\"><tr>\n";
  $navbar .= "<td width=\"0%\" align=\"center\">" . formButtonLink($Pages["Menu"], _("Menu")) . "</td>\n";
  $navbar .= "</tr></table>\n";
  // GRAPHIC STUFF

  return $navbar;
}

/**
 * Displays errors contained into
 * the error pipe.
 */
function formErrors()
{
  global $errorMsg;

  $errors = FALSE;

  if($errorMsg)
    {
      $errors = '<div id="Errors">' . "\n";
      $errors .= "<h2>" . myhtmlentities(_("ErrorMsg")) . "</h2>\n";
      $errors .= "<ul>\n";
      foreach($errorMsg as $msg)
	{
	  $errors .= "<li>" . $msg . "</li>\n";
	}
      $errors .= "</ul>\n";
      $errors .= "</div>\n";
    }

  return $errors;
}

/**
 * Displays the login password box used
 * for the authentification
 * There are error messages into the pipe, normally
 * it's error messages from a previous authentification
 * process that failed (login and password error)
 * Future version should distinguished login/password error (WARNING RETRY)
 * and Database connection error (FATAL CONTACT BEV)
 */
function formAuthBox($scriptName, $extraInputs = "")
{
  global $errorMsg;

  $auth = "<div id='formAuthBox'>\n";
  // check if there are errors
  $errors = formErrors($errorMsg);
  if($errors)
    {
      $auth .= formErrors($errorMsg);
    } 
  $auth .= '<form action="' . $scriptName . '" method="post">' . "\n";
  $auth .= '<table summary="Login form" cellspacing="10">' . "\n";
  $auth .= "<caption>" . myhtmlentities(_("Enter")) . "</caption>\n";
  $auth .= '<tr><td align="right"><h4>' . myhtmlentities(_("Login")) . ':</h4></td><td><input type="text" name="login" value="" maxlength="15" /></td></tr>' . "\n";
  $auth .= '<tr><td align="right"><h4>' . myhtmlentities(_("Passwd")) . ':</h4></td><td><input type="password" name="passwd" value="" maxlength="15" /></td></tr>' . "\n";
  $auth .= '<tr><td>&nbsp;</td><td><input name="submit" type="submit" value="' . myhtmlentities(_("Connect")) . '" /></td></tr>' . "\n";
  $auth .= "</table>\n$extraInputs</form>\n";
  $auth .= "</div>";

  return $auth;
}

/**
 * Returns the HTML code for a button
 * when pressed go to a specified URL.
 */
function formButtonLink($href, $text, $ext = "")
{
  $str = '<form action="' . $href . '" method="post" >' . "\n";
  $str .= '<button type="submit">' . "\n";
  $str .= "<strong>" . myhtmlentities($text) . "</strong>\n";
  $str .= "</button>\n";
  $str .= $ext;
  $str .= "</form>\n"; 
  return $str;
}

/**
 * Returns a href with the specified URL.
 * If newwindow is true then open a new window
 * with the specified parameters
 */
function getLink($href, $text, $newwindow = false, $id = "")
{
  $str = "";
  if($newwindow)
    {
      $str = "<a id=\"$id\" target=\"_blank\" rel='nofollow' href=\"$href\" >" . $text . "</a>\n";
    }
  else
    {
      $str = "<a id=\"$id\" href=\"" . $href . '">' . $text . "</a>\n";
    }
  return $str;
}

function getLinkOnClick($onClick, $text, $id = "")
{
  return "<a id=\"$id\" href=\"#\" onClick=\"$onClick\">" . $text . "</a>\n";
}

/**
 * Returns the HTML code for a button
 * with an onClick action
 */
function getButtonOnClick($onClick, $text)
{
  $str = '<button type="submit" onClick="' . $onClick . '">' . "\n";
  $str .= "<strong>" . myhtmlentities($text) . "</strong>\n" . "</button>\n" . "\n";
  return $str;
}

/**
 * Returns the HTML code for a button and an input
 * with an submit action
 */
function formInput($href, $text, $type, $paramName, $defaultValue )
{
  $str = '<form action="' . $href . '" method="GET">' . "\n" ;
  $str .= '<input type="' . $type . '" maxlength=10 size=10 name="' . $paramName . '" value="' . $defaultValue . '" />' ; 
  $str .= '<button type="submit">' . "<strong>" . myhtmlentities($text) . "</strong>\n" . '</button></form>' . "\n";

  return $str;
}

/**
 * Returns a drop down list box with the
 * specified array of elements
 */
function formDropDownList($href, $nameParam, $values, $selectedElt)
{
  $str = "<form action=\"" . $href . "\" method=\"post\">\n";
  $str .= "<select name=\"" . $nameParam . "\">\n";
  for($i = 0; $i < count($values); $i++)
    {
      $selected = "";
      if($selectedElt and $values[$i] == $selectedElt)
	{
	  $selected = "selected=\"selected\"";
	}
      $str .= "<option value=\"" . $values[$i] . "\" $selected>" . $values[$i] . "\n";
    }
  $str .= "</select><input type=\"submit\" value=\"Set\" /></form>\n";
  return $str;
}

/**
 * Returns the HTML code for a button and a select
 * with an submit action
 */
function formSelect($href, $nameParam, $values, $text, $defaultValue)
{
  $str = '<form action="' . $href . '" method="GET" >' . "\n";
  $str .= '<select name="' . $nameParam . '" >' . "\n";
  for($elem=0; $elem < count($values); $elem++)
    {
      $selected = "";
	if($values["$elem"] == $defaultValue)
	  {
	    $selected = "selected=\"selected\"";
	  }
	$str .= '<option name="' . $values["$elem"] . "\" $selected>"  . $values["$elem"] . "\n";
    }
  $str .=  '</select><button type="submit" >' . "<strong>" . myhtmlentities($text) . "</strong>\n" . '</button></form>' . "\n";

  return $str;
}


/**
 * Returns the HTML code for a checked button.
 */
function formCheckBox($href, $text, $ischecked)
{
  $str = '<form action="' . $href . ' method="post">';
  $str .= '<input type="checkbox" ';
  
  if($ischecked)
    {
      $str .= " checked ";
    }
  
  $str .= " />" . myhtmlentities($text) . "\n";
  $str .= "</form>\n";
  
  return $str;
}

/**
 * Returns the HTML code for an image
 * that is contained into the image list array
 */
function formImage($imageRef)
{
  global $Images;

  if(isset($Images[$imageRef]))
    {
  	$str = '<img src="' . BLUEPHPROOT . $Images[$imageRef] . '" alt="Logo" />';
    }
  else
   {
  	$str = '<img src="' . $imageRef . '" alt="Logo" />';
   }
  return $str;
}

/**
 * Returns the HTML code for date range selection
 * Must be the first form in the page because
 * javascript refers to forms[0] in the javascript code
 * another approach should be better ;-)
 */
function formDateRange($id, $href, $input1, $input2, $date1 = "", $date2 = "")
{
  $str = '<form action="' . $href . '" method="post">' . "\n";
  $str .= myhtmlentities(_('ForThePeriod')) . "\n";
  $str .= "<span id=\"inputDate\">\n" . myhtmlentities(_('PeriodFrom')) . " " . getInputDate($id, $input1, $date1) . "\n</span>\n";
  $str .= "<span id=\"inputDate\">\n" . myhtmlentities(_('PeriodTo')). " " . getInputDate($id, $input2, $date2) . "\n</span>\n";
  $str .= "<input type=\"submit\" value=\"" . myhtmlentities(_('Apply')) . "\" />\n";
  $str .= "</form>";
  return $str;
}

function formDate($id, $href, $input, $date)
{
  $str = '<form action="' . $href . '" method="post">' . "\n";
  $str .= myhtmlentities(_('ForTheDay')) . "\n";
  $str .= "<span id=\"inputDate\">\n" . getInputDate($id, $input, $date) . "\n</span>\n";
  $str .= "<input type=\"submit\" value=\"".myhtmlentities(_('Apply'))."\" />\n";
  $str .= "</form>";
  return $str;
}

function formButtonDate($id, $href, $label, $date)
{
  $str = '<form action="' . $href . '" method="post">';
  $str .= "<input type=\"hidden\" name=\"" . $id . "\" value=\"" . $date . "\" />";
  $str .= "<input type=\"submit\" value=\"" . $label . "\" />";
  $str .= "</form>\n";
  return $str;
}

function getFooter()
{
  $str = "</body>\n</html>\n";
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

function getInputText($inputName, $maxLength, $defaultValue = "")
{
  $str = '<input type="text" name="' . $inputName . '" value="' . $defaultValue 
    . '" maxlength="' . $maxLength . '" />' . "\n";
  return $str;
}

function getInputTextArea($inputName, $rows, $cols, $defaultValue = "")
{
  $str = '<textarea name="' . $inputName . '" rows="' . $rows 
    . '" cols="' . $cols . '">' . $defaultValue . '</textarea>' . "\n";
  return $str;
}

/**
 * \param id is the index of the form in the current page
 * example: id = 0 for the first form in the page, id = 1 for the second ...
 * so you can have differents form with the same inputName or one form with different inputName
 */
function getInputDate($id, $inputName, $defaultValue = "", $dateFormat = 'dd/MM/yyyy', $extraParams = "", $title = "Click to select the date from the calendar popup")
{
  $anchor = "anchor" . $id;

  $str = '<script type="text/javascript" language="JavaScript" >' . "\n";
  $str .= 'var cal1 = new CalendarPopup();' . "\n";
  $str .= '</script>' . "\n";
  $str .= '<input ' . $extraParams . ' type="text" id="' . $inputName . '" name="' . $inputName . '" value="' . $defaultValue . '" size="10" />' . "\n";
  $str .= '<a href="#" onClick="cal1.select(document.forms[' . $id . '].' .
    $inputName . ",'$anchor','" . $dateFormat . "'); " . 'return false;"' .  
    " title=\"$title\" name=\"$anchor\" id=\"$anchor\" alt=\"$title\">" . "\n";

  $str .= '<img class="date" width="18" height="18" src="' . BLUEPHPROOT . '/BluePHP/Graphics/date.gif" alt="Calendar image" /></a>';
  return $str;
}

function getInputHour($inputName, $defaultValue = "00", $extraParams = "")
{
  $str = '<select ' . $extraParams . ' name="' . $inputName . '">' . "\n";
  for($i = 0; $i < 24; ++$i)
    {
      $selected = "";
      $v = sprintf("%02d", $i);
      if($v == $defaultValue)
	{
	  $selected = "selected=\"selected\"";
	}
      $str .= "<option value=\"$v\" $selected>$v" . "h" . "</option>\n";
    }
  $str .= "</select>\n";
  return $str;
}

function getInputSelect($inputName, $connection, $tableName, $defaultValue = "")
{
  $str = '<input id="' . $inputName . '" type="text" name="' . $inputName . '" value="' . $defaultValue . '" />' . "\n";
  $str .= getSelect($inputName, $connection, $tableName, $defaultValue);
 return $str;
}

function getSelect($inputName, $connection, $tableName, $defaultValue = "")
{
  $str .= '<select name="' . $inputName . '" onChange="document.getElementById(' . "'" . $inputName . "'" .
    ').value=document.getElementById(' . "'" . $inputName . "'" . ').value">' . "\n";
  
  $query = "SELECT DISTINCT " . $inputName . " FROM " . $tableName .
    " WHERE " . $inputName . ' != "" ORDER BY ' . $inputName;
  $result = $connection->query($query);

  while($row = $result->nextAssoc())
    {
      $selected = "";
      if($row["$inputName"] == $defaultValue)
	{
	  $selected = "selected=\"selected\"";
	}
      $str .= '<option value="' . $row["$inputName"] . '" $selected>' . $row["$inputName"] . '</option>' . "\n";
    }
  $result->freeResult();

  $str .= "</select>\n";
 return $str;  
}

/**
 * Must change this name to
 * formButtonLink
 */
function getButtonLink($href, $text)
{
  $str = '<form action="' . $href . '" method="post" >' . "\n";
  $str .= '<button type="submit">' . "\n";
  $str .= "<strong>" . myhtmlentities($text) . "</strong>\n" . "</button>\n" . "</form>\n"; 

  return $str;
}

/**
 * The login password box that can appear at 2 stages
 * 1. At the user first access to the application
 * 2. When the user session has expired
 * $scriptName is used to tell where we have to return after the authentification process was done.
 * $wantHeader is used to tell whether or not we have to output a HTML page header, this araise
 * when the caller (i.e normally $scriptName) do not output a HTML page header (see main_export_all.php).
 */
function formAuth($scriptName, $wantHeader = FALSE, $title = false)
{
  if(!$title)
    {
      $title = TITLE;
    }
  $mainPage = new FormPage($title);
  $mainPage->topPart = formImage(LOGO);
  $mainPage->mainPart = formAuthBox($scriptName);
  if($wantHeader)
    {
      $mainPage->pageHead(CSS);
    }
  $mainPage->render();
  $mainPage->pageTail();
}

function getURLWithParams($base, $params)
{
  $str = $base;
  $isFirstParam = true;
  foreach($params as $key => $value)
    {
      if($isFirstParam)
	{
	  $isFirstParam = false;
	  $str .= "?";
	}
      else
	{
	  $str .= "&";
	}
      $str .= $key . "=" . $value;
    }
  return $str;
}

/**
 * Automatic menu generation from an associative array in the form
 * { key1 => title1, key2 => title2, ... }
 * The key is the id menu entry and title is the menu entry label displayed.
 * If the key does no start with the _ character then it is considered as the href 
 * otherwise the menu entry does not contain an anchor and just display the title.
 *
 * The extra parameters are used to make some submenu where the entry is passed as a URL parameter.
 */
function formMenu($menu, $current, $params = false, $paramsKey = false, $paramsURLBase = false)
{
  if(!is_array($menu))
    {
      return "";
    }
  $str = "<ul>\n";
  foreach($menu as $key => $title)
    {
      if($current == $key)
	{
	  $title = myhtmlentities($title);
	  $str .= "<li class=\"current\"><a class=\"current\" href=\"#\">$title</a></li>\n";
	}
      else if(is_array($params))
	{
	  $title = myhtmlentities($title);
	  $params[$paramsKey] = $key;
	  $str .= "<li><a href=\"" . getURLWithParams($paramsURLBase, $params) . "\">$title</a></li>\n";
	}
      else if(substr($key, 0, 1) == "_")
	{
	  if(is_array($title))
	    {
	      $str .= "<li id=\"" . $title[0] . "\">" . myhtmlentities($title[1]) . "</li>\n";
	    }
	  else
	    {
	      $str .= "<li>" . myhtmlentities($title) . "</li>\n";
	    }
	}
      else if(substr($key, 0, 1) == "!")
	{
	  $str .= "<li><a href=\"". $title[0] . "\" rel=\"nofollow\" target=\"_blank\">" . $title[1] . "</a></li>\n";
	}
      else
	{
	  $title = myhtmlentities($title);
	  $str .= "<li><a href=\"$key\">$title</a></li>\n";
	}
    }
  $str .= "</ul>\n";
  return $str;
}

function getSelectFromArray($name, $options, $default, $extra = "")
{
  $str = "<select name=\"$name\" $extra>\n";
  foreach($options as $k => $v)
    {
      $selected = "";
      if($k == $default)
	{
	  $selected = "selected=\"selected\"";
	}
      $str .= "<option value=\"$k\" $selected>". myhtmlentities(_($v)) ."</option>\n";
    }
  $str .= "</select>\n";
  return $str;
}

?>
