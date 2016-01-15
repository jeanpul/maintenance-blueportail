<?php

/**
 * CHANGE THE VALUE TO YOUR APPROPRIATE SETTING
 * LANG : Application language
 * Language translation is done via locales using mo files
 * @see mkmf commands @see po sub directory
 */
define ('LANG', "en_US");

setlocale(LC_ALL, LANG);

/**
 * CHANGE THE VALUES TO YOUR APPROPRIATE SETTINGS
 * DBHOST : hostname of the machine which have the database
 * DBNAME : database name (table name in SQL)
 * DBUSER and DBPASSWD : user login and password that have i/o access to the database
 */
define ('BASEPATH', "/home/DATA/");
define ('BTOP_SERVER_DBPATH', BASEPATH . "BTopLocalServer/");

define ('TMP_COUNTER_FILE', BTOP_SERVER_DBPATH . "counters.query");
define ('TMP_NUMBERING_FILE', BTOP_SERVER_DBPATH . "numbering.query");

define ('COUNTDBTYPE', "SQLITE3");
define ('COUNTDBHOST', "localhost");
define ('COUNTDBNAME', BTOP_SERVER_DBPATH . "counters.db");
define ('COUNTDBUSER', "blue");
define ('COUNTDBPASSWD', "blue4eye");

define ('CONFIGDBTYPE', "SQLITE3");
define ('CONFIGDBHOST', "localhost");
define ('CONFIGDBNAME', BTOP_SERVER_DBPATH . "processed.db");
define ('CONFIGDBUSER', "blue");
define ('CONFIGDBPASSWD', "blue4eye");

define ('DATADBTYPE', "SQLITE3");
define ('DATADBHOST', "localhost");
define ('DATADBNAME', BTOP_SERVER_DBPATH . "BLC.db");
define ('DATADBUSER', "blue");
define ('DATADBPASSWD', "blue4eye");
define ('DATADBPATH', BTOP_SERVER_DBPATH);

?>