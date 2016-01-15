<?php

function defaultHtmlPageStartParams()
{
    return array('pageStartMainTitle' => "MAIN TITLE",
		 'pageStartPageTitle' => "PAGE TITLE",
		 'pageStartVersionRef' => "VERSION REFERENCE",
		 'pageStartHeadDataExt' => "",
		 'pageStartLogoURL' => "/BlueSystem/styles/LogoStyle.gif",
		 'pageStartCSS' => "/BlueSystem/styles/StyleBlueEyeVideo.css");
}

/**
mainTitle
pageTitle
versionRef
headDataExt
*/
function htmlPageStart($params){

    $str = '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
 <head>
  <title>';
    $str .= $params['pageStartMainTitle'];
    $str .= '</title>
  <meta http-equiv="content-type" content="text/html; charset=ISO-8859-1" />
  <link rel="stylesheet" type="text/css" href="' . $params['pageStartCSS'] . '"/>';
    $str .= $params['pageStartHeadDataExt'];
    $str .= '
 </head>
 <body>
  <div id="main">
   <div id="top">
    <table class="head">
     <tr>
      <td><div id="logo"><img src="' . $params['pageStartLogoURL'] . '" alt="logo BEV"/></div></td>
      <td><div align="right"><h3>';
$str .= $params['pageStartVersionRef'];
$str .= '</h3></div></td>
    </tr>
   </table>
  </div>
  <div id="desc">

   <h1>';

$str .= $params['pageStartPageTitle'];
$str .= '</h1>
';

    return $str;
}

function defaultHtmlPageEndParams()
{
    return array('pageEndCopyright' => 'Copyright message');
}

function htmlPageEnd($params)
{
  $str = '
   </div>
   <div id="foot">
    <p>';
  $str .= $params['pageEndCopyright'];
  $str .= '</p>
   </div>
  </div>
 </body>
</html>';
   return $str;
}

function getInfoMessage($msg)
{
  $str  = "<div id=\"info\">\n";
  $str .= " <p>$msg</p>\n";
  $str .= "</div>\n";
  return $str;
}

function getErrorMessage($msg)
{
  $str  = "<div id=\"error\">\n";
  $str .= " <h1><u>"._("Error message:") . "</u></h1>\n";
  $str .= " <p>$msg</p>\n";
  $str .= "</div>\n";
  return $str;
}

function defaultGetBackButtonParams()
{
    return array('backButtonTitle' => "BACK TITLE",
		 'backButtonUrl' => "",
		 'backButtonFormComp' => "");
}

/**
 * - backButtonTitle
 * - backButtonUrl
 * - backButtonFormComp
 */
function getBackButton($params)
{
  $str = "<div id=\"barHome\">\n";
  $str .= " <form action=\"" . $params['backButtonUrl'] . "\" method=\"get\">\n";
  $str .= $params['backButtonFormComp'];             
  $str .= "  <button id=\"Home\" type=\"submit\" value=\"".$params['backButtonTitle']."\">" . $params['backButtonTitle'] . "</button>\n";
  $str .= " </form>\n";
  $str .= "</div>\n";
  return $str;
}

function defaultEmptyColProperties($nbcol)
{
    $p = array();
    for($i = 0; $i < $nbcol; $i++)
    {
	array_push($p, "");
    }
    return $p;
}

/**
 *
 */
function getDataArray($data, $coltitles, $colproperties = false)
{
    $str = '<table class="classic">'."\n";
    if($coltitles)
    {
	$str .= " <tr>";
    foreach($coltitles as $col)
    {
	$str .= "<th>$col</th>";
    }
    $str .= "</tr>\n";
    }

    foreach($data as $row)
    {
	$idx = 0;
	$str .= " <tr>";
	foreach($row as $c)
	{
	    $str .= "<td";
	    if($colproperties) $str .= " " . $colproperties[$idx];
	    $str .= ">" . $c . "</td>";
	    $idx++;
	}
	$str .= "</tr>\n";
    }
    $str .= "</table>\n";
    return $str;
}

function getLink($url, $title)
{
    $str = "<a href=\"$url\">$title</a>";
    return $str;
}  

function getTextFieldWithTitle($title, $var, $value = false)
{
    return "<p>".$title." " . Forms_inputText($var, $value)."</p>";
}

function applyButton($button_name)
{
  return '<button id="apply" type="submit" value="' . $button_name . '" >' . $button_name . '</button>' . "\n";
}

function getActiveIconElement($url, $title, $cssid = "")
{
  $str = '<a ';
  if($cssid != "")
    {
      $str .= 'id="' . $cssid . '" ';
    }
  $str .= 'href="' . $url . '">' . $title . '</a>';
  return $str;
}

function getInactiveIconElement($title, $cssid = "")
{
  $str = '<span ';
  if($cssid != "") {
    $str .= ' id='. $cssid;
  }
  $str .= ' >' . $title . '</span>';
  return $str;
}

function getIconSet($elements)
{
  $str = '<table class="classic">
 <tr>
';
  foreach($elements as $col)
    {
      $str .= '  <td><div class="linkList"><ul>';
      foreach($col as $row)
	{
	  $str .= '<li>';
	  $str .= $row;
	  $str .= "</li>\n";
	}

      $str .= '    </ul></div></td>';
    }
  $str .= ' </tr>
</table>
';
  return $str;
}

function testParams($required_keys, $params){
    foreach($required_keys as $k)
    {
	if(!array_key_exists($k, $params)) return false;
    }
	return true;
}

function errorMessageAndExit($params, $msg)
{
  $params['errorPageMessage'] = $msg;
  echo BSGTemplate_errorPage($params);
  exit(0);
}

function infoMessageAndExit($params, $msg)
{
  $params['infoPageMessage'] = $msg;
  echo BSGTemplate_infoPage($params);
  exit(0);
}



?>
