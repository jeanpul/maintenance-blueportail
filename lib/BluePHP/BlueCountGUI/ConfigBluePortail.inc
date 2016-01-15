<?php

/**
 * CHANGE THE VALUE TO YOUR APPROPRIATE SETTING
 * LANG : Application language
 * Language translation is done via locales using mo files
 * @see mkmf commands @see po sub directory
 */

define('TEXT_DOMAIN', "BlueCountGUI");

textdomain("BlueCountGUI");

/**
 * Where to find the documentations ?
 */
define ('DOCPATH', "/Docs/");

/**
 * Main logo
 */
define ('MAXLOGOSIZE', 60000);
define ('MAXLOGOWIDTH', 1024);
define ('MAXLOGOHEIGHT', 768);

/**
 * Graphics
 */
//define ('GRAPHIC_KIND', "GNUPLOT");
define ('GRAPHIC_KIND', "OPENFLASHCHART");
//define ('GRAPHIC_KIND', "SWFCHART");
if(GRAPHIC_KIND == "SWFCHART")
{
  define ('SWFCHART_PATH', "/BluePHP/charts_php/");
  define ('SWFCHART_BGCOLOR', "ffffff");
  define ('SWFCHART_KEY', "H1XVCFJNW-HLN2I5T4Q79KLYCK07EK");
}


/**
 * Menu pages
 */
$Menu = array( getBluePortailURL("CountingGeneric.php") => ("Analysis"));

/**
 * Default page
 * Do not use the name of the menu entry in case
 * where you don't have a menu or if you modify the
 * Menu entry title.
 */
define ('DEFAULTPAGE', getBluePortailURL("index.php") );

define ('SESSION_DBTYPE', "SQLITE3");
define ('SESSION_DBHOST', "localhost");
define ('SESSION_DBNAME', BASEPATH . "BlueCountGUI/session.db");
define ('SESSION_DBUSER', "blue");
define ('SESSION_DBPASSWD', "blue4eye");

/** 
 * Here is the minimum access level required to be an admin user 
 */
define('CONSULTACCESS', 1);
define('ADMINACCESS', 2);


/**
 * In the following arrays the values are translated after the loading of the user language.
 * It is now done in the file preInc.inc
 * To save time, the value are set in english withtout a useless try for translation.
 */

$EntitiesAnalysis = array ( "general/" => "General", 
			    "flows" => "In/Out flows",
			    "visitors" => "Visitors",
			    "occupancy" => "Occupancy",
			    "numbering" => "Numbering",
			    "waitingTime" => "Waiting Time",
			    "comp/" => "Comparisons",
			    "flowDoorComp" => "In/Out flows",
			    "visLocComp" => "Visitors",
			    "numberingLocComp" => "Numbering");

$Analysis = array ( "flows" => "In/Out flows",
		    "visitors" => "Visitors",
		    "occupancy" => "Occupancy",
		    "numbering" => "Numbering",
		    "waitingTime" => "Waiting Time",
		    "flowDoorComp" => "Doors flows comparisons",		
		    "visLocComp" => "Visitors locations comparisons",
		    "numberingLocComp" => "Location numbering comparisons" );

$AnalysisParams = array ( "waitingTime" => "WaitingTime_steps_%s.inc" );

$ExtraParams = array( "displayPeriods" => array( 'hour' => array('minute' => "Hour / 1 minute",
								 '5minute' => "Hour / 5 minutes"),
						 'day' => array('hour' => "Day"),
						 'month' => array('day' => "Month"),
						 'year' => array('month' => "Year")
						 ));
?>
