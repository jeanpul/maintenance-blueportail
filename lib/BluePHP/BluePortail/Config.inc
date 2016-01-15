<?php

/**
 * Displays all errors and warnings
 */
ini_set('display_errors', true);

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
 * Changes default path directory to include
 * automatically files from BluePHP library
 */
function loadBluePHP()
{
  ini_set('include_path', ini_get('include_path') . ":/var/www/html/:/var/www/");
}

/**
 * CHANGE THE VALUE TO YOUR APPROPRIATE SETTING
 * LANG : Application language
 * Language translation is done via locales using mo files
 * @see mkmf commands @see po sub directory
 */
define ('LANG', "en_US");
define ('TEXT_DOMAIN', "BluePortail");


/**
 * Software version @see svn info :
 */
define ('SOFTNAME', "BluePortail GUI"); 
define ('VERSION', "1758");

/**
 * Where to find the documentations ?
 */
define ('DOCPATH', "/Docs/");

/**
 * Main title
 */
define ('TITLE', "BluePortail");

/**
 * Bottom sentence
 */
define ('COPYRIGHT', "Copyright 2007 Blue Eye Video All Rights Reserved");

define('SESSIONTIME', 3600);
define('CSS', "styles/blueeyevideo.css");

/**
 * Main logo
 */
define ('LOGO', "BEV_LOGO_MR");
define ('MAXLOGOSIZE', 60000);
define ('MAXLOGOWIDTH', 1024);
define ('MAXLOGOHEIGHT', 768);

/**
 * Menu pages
 * MUST BE DEFINED AFTER THE LOCALE DEFINITION 
 */
$Menu = array( "index.php" => _("Main menu"));

/**
 * Default page
 * Do not use the name of the menu entry in case
 * where you don't have a menu or if you modify the
 * Menu entry title.
 */
define ('DEFAULTPAGE', "index.php" );

/**
 * CHANGE THE VALUES TO YOUR APPROPRIATE SETTINGS
 * DBHOST : hostname of the machine which have the database
 * DBNAME : database name (table name in SQL)
 * DBUSER and DBPASSWD : user login and password that have i/o access to the database
 */
define ('CONFIGDBTYPE', "SQLITE3");
define ('CONFIGDBHOST', "localhost");
define ('CONFIGDBNAME', "/home/DATA/BluePortail/session.db");
define ('CONFIGDBUSER', "blue");
define ('CONFIGDBPASSWD', "blue4eye");

define ('CONFIGDBNAME', "/home/DATA/BluePortail/session.db");

define ('SESSION_DBTYPE', "SQLITE3");
define ('SESSION_DBHOST', "localhost");
define ('SESSION_DBNAME', "/home/DATA/BluePortail/session.db");
define ('SESSION_DBUSER', "blue");
define ('SESSION_DBPASSWD', "blue4eye");

/**
 * Here is the minimum access level
 * required to be an admin user
 */
define ('CONSULTACCESS', 1);
define ('ADMINACCESS', 2);

/**
 * where to find executable scripts
 */
define ('SCRIPTSPATH', "/var/www/BluePortailAdmin/scripts");

define('KEYPATH', "/home/DATA/BluePortail/keys/");

define('CLIENTHOSTFILE', "/home/DATA/BluePortail/clientDest.txt");

?>
