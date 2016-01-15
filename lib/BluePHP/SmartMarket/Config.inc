<?php

include_once("BluePHP/BTopLocalServer/Config.inc");

/**
 * Displays all errors and warnings
 */
//error_reporting(E_ALL);
//ini_set('display_errors', 'On');
//ini_set('error_reporting', E_ALL | E_STRICT);

//date_default_timezone_set("UTC");

/**
 * Charset est maintenant lié à l'application
 * et n'est plus définit dans Language.inc du BluePHP
 */
if(!defined("LANG_CHARSET"))
  {
    define('LANG_CHARSET', "UTF-8");
  }

/**
 * Check if BluePortail is activated through the
 * clientBluePortail parameter
 */
$clientBluePortail = false;
if(isset($_REQUEST) and isset($_REQUEST["clientBluePortail"]))
{
  $clientBluePortail = $_REQUEST["clientBluePortail"];
  include_once(BLUEPORTAILROOT . "/" . $_REQUEST["clientBluePortail"] . "/SmartMarket/" . "ConfigBluePortail.inc");
}
else
{
  include_once("ConfigBluePortail.inc");
}

/**
 * Returns the client id if exist
 */
function getClientId()
{
  $bsToolsFile = "/etc/BEV/phpScripts/BlueSystemToolsPHP/BSbasetools.inc";
  if(file_exists($bsToolsFile))
    {
      include_once($bsToolsFile);
      return BSbasetools_obtainHostClientId();
    }
  else
    {
      return "4b5f9fd8";
    }
}

?>
