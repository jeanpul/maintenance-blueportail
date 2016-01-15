<?php

include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/BTopLocalServer/Config.inc");

/**
 * Application name
 */
define ('SOFTNAME', "QMS: Statistics");

/**
 * Application version
 */
define('VERSION', '');

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
  include_once(BLUEPORTAILROOT . "/" . $_REQUEST["clientBluePortail"] . "/BlueCountGUI/" . "ConfigBluePortail.inc");
}
else
{
  include_once("ConfigBluePortail.inc");
}

?>