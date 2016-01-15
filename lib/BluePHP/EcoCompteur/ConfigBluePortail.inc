<?php

/**
 * CHANGE THE VALUE TO YOUR APPROPRIATE SETTING
 * LANG : Application language
 * Language translation is done via locales using mo files
 * @see mkmf commands @see po sub directory
 */
define ('LANG', "en_US");

setlocale(LC_ALL, LANG);

textdomain("EcoCompteur");

define('TIMEZONE', "Europe/Paris");

date_default_timezone_set(TIMEZONE);

/**
 * Software version @see svn info :
 */
define ('SOFTNAME', "EcoCompteur Configuration"); 
define ('VERSION', "ND");

/**
 * Where to find the documentations ?
 */
define ('DOCPATH', "/Docs/");

/**
 * Main title
 */
define ('TITLE', "Configuration");

/**
 * Bottom sentence
 */
define ('COPYRIGHT', "Copyright 2010 Blue Eye Video All Rights Reserved");

define('CSS', "styles/blueeyevideo.css");

/**
 * Main logo
 */
define ('LOGO', "BEV_LOGO_MR");

/**
 * CHANGE THE VALUES TO YOUR APPROPRIATE SETTINGS
 * DBHOST : hostname of the machine which have the database
 * DBNAME : database name (table name in SQL)
 * DBUSER and DBPASSWD : user login and password that have i/o access to the database
 */
define ('BASEPATH', "/home/DATA/");
define ('SM_SERVER_DBPATH', BASEPATH . "EcoCompteur/");

define ('DATADBTYPE', "SQLITE3");
define ('DATADBHOST', "localhost");
define ('DATADBNAME', SM_SERVER_DBPATH . "data.db");
define ('DATADBUSER', "blue");
define ('DATADBPASSWD', "blue4eye");
define ('DATADBPATH', SM_SERVER_DBPATH);

?>
