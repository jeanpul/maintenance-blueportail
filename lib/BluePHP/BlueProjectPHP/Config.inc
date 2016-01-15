<?php

header("Content-type: text/html; charset=utf-8");

/**
 * Displays all errors and warnings
 */
error_reporting(E_ALL ^ E_DEPRECATED);
//ini_set('error_reporting', E_ALL);
//ini_set('display_errors', true);

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
 * CHANGE THE VALUE TO YOUR APPROPRIATE SETTING
 * LANG : Application language
 * Language translation is done via locales using mo files
 * @see mkmf commands @see po sub directory
 */
define ('LANG', "fr_FR");

setlocale(LC_ALL, LANG . ".UTF-8");

date_default_timezone_set("Europe/Paris");

/**
 * Main title
 */
define ('TITLE', "CIR Generator");

/**
 * Bottom sentence
 */
define ('COPYRIGHT', "Copyright 2009 Blue Eye Video All Rights Reserved");

define('CSS', "styles/blueeyevideo.css");

/**
 * Default page
 * Do not use the name of the menu entry in case
 * where you don't have a menu or if you modify the
 * Menu entry title.
 */
define ('DEFAULTPAGE', "index.php" );

/**
 * Base de données pour retrouver la liste des produits...
 */
define ('CONFIGDBTYPE', "MYSQL");
define ('CONFIGDBHOST', "pong");
define ('CONFIGDBNAME', "BlueProject");
define ('CONFIGDBUSER', "blue");
define ('CONFIGDBPASSWD', "blue4eye");

define("DB_SERVER", "pong");
define("DB_USER", "blue"); 
define("DB_PASSWD", "blue4eye"); 
define("DB_NAME", "BlueProject");

define('DOCROOT', '/net/injector/volume1/Sales_Mkg/Syno_Gestion Commerciale');

?>