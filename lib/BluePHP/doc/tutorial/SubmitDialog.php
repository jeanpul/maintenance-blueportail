<?php

include_once("BluePHP/BlueSystem/GUI/BSApplication.inc");
include_once("BluePHP/BlueSystem/GUI/BSDialogSubmit.inc");

/**
 * Simply create the Application and pass the Dialog
 * as the root window.
 */
$app = new BSApplication(new BSDialogSubmit("hello", 
					    array( "title" => "Hello world message", 
						   "msg" => "Hello world !" )));
/**
 * Show everything
 */
$app->render();
?>
