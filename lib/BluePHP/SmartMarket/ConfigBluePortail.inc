<?php

/**
 * CHANGE THE VALUE TO YOUR APPROPRIATE SETTING
 * LANG : Application language
 * Language translation is done via locales using mo files
 * @see mkmf commands @see po sub directory
 */
textdomain("SmartMarket");

/**
 * Software version @see svn info :
 */
define ('SOFTNAME', "SmartMarket Configuration"); 
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

define('SESSIONTIME', 3600);
define('CSS', "styles/blueeyevideo.css");

/**
 * Main logo
 */
define ('LOGO', "BEV_LOGO_MR");

/**
 * Menu pages
 * MUST BE DEFINED AFTER THE LOCALE DEFINITION 
 */
$Menu = array( "Alarms.php" => _("Alarms") );

/**
 * Default page
 * Do not use the name of the menu entry in case
 * where you don't have a menu or if you modify the
 * Menu entry title.
 */
define ('DEFAULTPAGE', "/cgi-bin/BTopServerMenu.cgi" );

/**
 * CHANGE THE VALUES TO YOUR APPROPRIATE SETTINGS
 * DBHOST : hostname of the machine which have the database
 * DBNAME : database name (table name in SQL)
 * DBUSER and DBPASSWD : user login and password that have i/o access to the database
 */
//define ('BASEPATH', "/home/DATA/");
define ('SM_SERVER_DBPATH', BASEPATH . "SmartMarket/");

//define ('DATADBTYPE', "SQLITE3");
//define ('DATADBHOST', "localhost");
//define ('DATADBNAME', SM_SERVER_DBPATH . "data.db");
//define ('DATADBUSER', "blue");
//define ('DATADBPASSWD', "blue4eye");
//define ('DATADBPATH', SM_SERVER_DBPATH);

/**
 * Parameters for the communication between
 * SmartMarket and the BlueSMS API
 */
define ('BLUESMSSERVER' , 'localhost');
define ('BLUESMSID', 'toto');
define ('BLUESMSSRC', 'NA');

/**
 * Global mail parameters
 */
define ('BLUEMAILSERVER' , 'localhost');
define ('BLUEMAILID', 'fabien.pelisson@gmail.com');
define ('BLUEMAILSRC', 'NA');


?>
