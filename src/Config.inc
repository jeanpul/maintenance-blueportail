<?php

error_reporting(E_ALL);

set_include_path("../lib" . PATH_SEPARATOR . get_include_path());
$zcpExcluded = array( "CRTBretagne" => array( 9 ) );

define('CLIENTHOSTFILE', '/home/DATA/BluePortail/clientDest.txt');
define('SCRIPTSPATH', "../scripts/");

?>