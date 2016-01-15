<?php

include_once("BluePHP/BlueSystem/GUI/BSApplication.inc");
include_once("BluePHP/BlueSystem/GUI/BSDialogInfo.inc");

/**
 * Simply create the Application and pass the Dialog
 * as the root window.
 */
$app = new BSApplication(new BSDialogInfo("hello", 
					  array( "title" => "Hello world message", 
						 "msg" => "Hello world !" ),
					  null));
/**
 * Show everything
 */
$app->render();
?>
