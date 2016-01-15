<?php

include_once("BluePHP/GUI/Button.inc");
include_once("BluePHP/GUI/Concat.inc");
include_once("BluePHP/BlueSystem/GUI/BSApplication.inc");
include_once("BluePHP/BlueSystem/GUI/BSDialogInfo.inc");

$button = new Button("bt_hello", array( "label" => "Click me" ));
$dialog = new BSDialogInfo("hello",
			   array( "title" => "Hello world message",
				  "msg" => "Hello world !" ),
			   null);
// avoid the dialog to be shown when the application render
$dialog->setAutoOpen(false);
// avoid the dialog to be destroyed when the user click the close button
$dialog->setDestroyAtClose(false);

$app = new BSApplication();

// concat simply put elements one after the other
$frame = new Concat(array($button, $dialog));

// the root window is always needed
$app->setRootWindow($frame);

// connect the button click to the dialog show property
// only when everything is rendered
$app->pushReadyJS($button->eventClick($dialog->eventOpen()));

$app->render();

?>
