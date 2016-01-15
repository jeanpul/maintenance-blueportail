<?php

/**
 * Displays all errors and warnings
 */
//ini_set('display_errors', true);

/**
 * Charset est maintenant lié à l'application
 * et n'est plus définit dans Language.inc du BluePHP
 */
if(!defined("LANG_CHARSET"))
  {
    define('LANG_CHARSET', "UTF-8");
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
  include_once(BLUEPORTAILROOT . "/" . $_REQUEST["clientBluePortail"] . "/BTopLocalServer/" . "ConfigBluePortail.inc");
}
else
{
  include_once("ConfigBluePortail.inc");
}

if(!defined('STATURL'))
{
  define ('STATURL', "/BlueCountGUI/index.php");
}

/**
 * Check if BlueSystemTools exist
 * so you can use the id client, ref number ...
 */
$haveBlueSystemTools = false;
if(file_exists("/etc/BEV/phpScripts/BlueSystemToolsPHP/BSConstant.inc"))
  {
    ini_set('include_path', ini_get('include_path') . ":/etc/BEV/phpScripts/");
    $haveBlueSystemTools = true;
  }

define('BEVLOGSROOT', "/var/log/bev"); 

?>