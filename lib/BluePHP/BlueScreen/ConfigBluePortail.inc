<?php

/**
 * CHANGE THE VALUE TO YOUR APPROPRIATE SETTING
 * LANG : Application language
 * Language translation is done via locales using mo files
 * @see mkmf commands @see po sub directory
 */
define ('LANG', "en_US");

setlocale(LC_ALL, LANG);

textdomain("BlueScreen");

/**
 * Software version @see svn info :
 */
define ('SOFTNAME', "BlueScreen Configuration"); 
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
define ('COPYRIGHT', "Copyright 2011 Blue Eye Video All Rights Reserved");

define('SESSIONTIME', 3600);
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
define ('BS_BASEPATH', BASEPATH . "BlueScreen/");

define ('DATADBTYPE', "SQLITE3");
define ('DATADBHOST', "localhost");
define ('DATADBNAME', BS_BASEPATH . "data.db");
define ('DATADBUSER', "blue");
define ('DATADBPASSWD', "blue4eye");
define ('DATADBPATH', BS_BASEPATH);

?>
