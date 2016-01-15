<?php

// main configuration file for BAM

/**
 * Displays all errors and warnings
 */
//error_reporting(E_ALL);
//ini_set('display_errors', 'On');
//ini_set('error_reporting', E_ALL | E_STRICT);

date_default_timezone_set("UTC");

/**
 * Charset est maintenant lié à l'application
 * et n'est plus définit dans Language.inc du BluePHP
 */
if(!defined("LANG_CHARSET"))
  {
    define('LANG_CHARSET', "UTF-8");
  }

/**
 * CHANGE THE VALUE TO YOUR APPROPRIATE SETTING
 * LANG : Application language
 * Language translation is done via locales using mo files
 * @see mkmf commands @see po sub directory
 */
define ('LANG', "en_US");

setlocale(LC_ALL, LANG);

textdomain("BAM");

/**
 * CHANGE THE VALUES TO YOUR APPROPRIATE SETTINGS
 * DBHOST : hostname of the machine which have the database
 * DBNAME : database name (table name in SQL)
 * DBUSER and DBPASSWD : user login and password that have i/o access to the database
 */
define ('BASEPATH', "/home/DATA/");
define ('BAM_DBPATH', BASEPATH . "BAM/");

define ('DATADBTYPE', "SQLITE3");
define ('DATADBHOST', "localhost");
define ('DATADBNAME', BAM_DBPATH . "data.db");
define ('DATADBUSER', "blue");
define ('DATADBPASSWD', "blue4eye");
define ('DATADBPATH', BAM_DBPATH);

?>
