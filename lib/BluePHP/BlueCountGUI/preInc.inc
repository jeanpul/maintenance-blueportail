<?php

/**
 * \file posInc.inc
 * Will be included at the begining of every page.
 * "Config.inc" must exits.
 * Do not forget to include postInc.inc at
 * the end of the page to render everything.
 */

session_start();

include_once('Config.inc');
include_once("BluePHP/Utils/Connect.inc");
include_once("BluePHP/Utils/Session.inc");
include_once("BluePHP/Utils/Forms.inc");
include_once("BlueCountGUILang.inc");
include_once("BluePHP/Utils/DateOps.inc");
include_once("BluePHP/Utils/Languages.inc");
include_once('CommonOps.inc');

// WARNING: will be removed at end
// so every page must define the pageLevel
// variable
$pageLevel = 1;
//
//if(!$pageLevel)
//{
//  $pageLevel = 1;
//}
//
// create connexion to the Configuration data
$glang = new BlueCountGUILang();

// shared parameters
$configSharedData = $glang->getSharedConfigData();

//create define on shared config data
define('COPYRIGHT', $configSharedData['COPYRIGHT']['value']);
define('SESSIONTIME', $configSharedData['SESSIONTIME']['value']);
define('CSS', $configSharedData['CSS']['value']);

// Change the TimeZone for Date and Time operations
// to match the BTopLocalServer TimeZone
date_default_timezone_set($glang->getTimeZone());

$session = checkAccess($pageName, $_POST, session_id(), TRUE, $pageLevel);

$title = $configSharedData["SITE_TITLE"]["value"];
$css = CSS;

// use global default language
setLanguage($configSharedData["LANG"]["value"]);

// check if the page should be in HTTPS or HTTP
$isHTTPS = false;
if(isset($_SERVER) and
   ((isset($_SERVER["SERVER_NAME"]) and
     $_SERVER["SERVER_NAME"] == "www.countingsolutions.net") or 
    (isset($_SERVER["HTTP_X_FORWARDED_HOST"]) and $_SERVER["HTTP_X_FORWARDED_HOST"] == "www.countingsolutions.net")))
  {
    // in that case every links
    // with http in it must be replaced by https
    $isHTTPS = true;
  }

if(!$session)
{
  //avoid warning for inexistant file (jscalendar) if lang not declared
  //calendar-en exist so no message
  $lang = "en";

  include_once("headerPart.inc");

  echo '<div id="Logo"><img src="styles/' . $configSharedData["LOGO_RELATIVE_PATH"]["value"] . '" alt="Logo" ></img></div>' . "\n";
  echo '<div class="MainPart">' . "\n";
  echo getFormAuthBox($pageName, getBluePortailInputs());
  echo "</div>\n";
  echo "</body>\n";
  echo "</html>\n";
  exit(1);
}

// user private parameters
$configData = $glang->getConfigData(array( "login" => $session["login"], 
					   "access" => $session["access"], 
					   "configSection" => "Config", 
					   "userSection" => "User" ) );

// user private indicators
$indicatorData = $glang->getConfigData(array ( "login" => $session["login"],
					       "access" => $session["access"],
					       "configSection" => "IndicatorsEnabled", 
					       "userSection" => "User" ) );

// create connexion to the BlueCount data
//$clang = new BlueCountLang();

// ** USE USER PREFERENCES **
// use user defined language

setLanguage($configData["LANG"]["value"]);

// user css
$css = $configData['CSS']['value'];

// user title
$title = $configData['SITE_TITLE']["value"];

translateArrayTerms();

?>

<?php

$headerExt = "";

if($configData["SESSIONTIME"]["value"] > 0)
{
  $headerExt = $headerExt . '<meta http-equiv="refresh" content="' . $configData["SESSIONTIME"]["value"] . "\"/>\n";
}

//Language for the calendar
$data = explode("_", $configData["LANG"]["value"], 2);
if(count($data) > 1)
  {
    $ignore = $data[1];
  }
$lang = $data[0];

include_once("headerPart.inc");

?>

