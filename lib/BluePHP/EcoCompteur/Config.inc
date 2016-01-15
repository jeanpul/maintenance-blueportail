<?php

/**
 * Displays all errors and warnings
 */
error_reporting(E_ALL);
ini_set('display_errors', 'On');
ini_set('error_reporting', E_ALL | E_STRICT);

/**
 * Used by BluePHP to display accessible url links into
 * its directories. Mainly for graphics elements into
 * the BluePHP/Graphics directory. For example
 * BluePHP/Graphics/toto.gif will be replaced
 * by BLUEPHPROOT/BluePHP/Graphics/toto.gif
 * An empty value means accessible directly from root /
 */
define ('BLUEPHPROOT', "");

/**
 * Charset est maintenant lié à l'application
 * et n'est plus définit dans Language.inc du BluePHP
 */
if(!defined("LANG_CHARSET"))
  {
    define('LANG_CHARSET', "UTF-8");
  }

/**
 * Changes default path directory to include
 * automatically files from BluePHP library
 */
function loadBluePHP()
{
  ini_set('include_path', ini_get('include_path') . ":/var/www/html/:/var/www/");
}

/**
 * User by BluePortail to provide multiple BlueCouncGUI with
 * multiple databases and configuration files.
 */
//define ('BLUEPORTAILROOT', "/var/www/BluePortail/" );
define ('BLUEPORTAILROOT', "/home/DATA/BluePortail/clients/" );

/**
 * Check if BluePortail is activated through the
 * clientBluePortail parameter
 */
$clientBluePortail = false;
if(isset($_REQUEST) and isset($_REQUEST["clientBluePortail"]))
{
  $clientBluePortail = $_REQUEST["clientBluePortail"];
  include_once(BLUEPORTAILROOT . "/" . $_REQUEST["clientBluePortail"] . "/EcoCompteur/" . "ConfigBluePortail.inc");
}
else
{
  include_once("ConfigBluePortail.inc");
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
      $data = explode("?", $url, 2);
      $url = $data[0] . "?clientBluePortail=" . $clientBluePortail;
      if(isset($data[1]))
	{
	  $url .= "&" . $data[1];
	}
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

if(!defined('STATURL'))
{
  define ('STATURL', "/BlueCountGUI/index.php");
}

?>
