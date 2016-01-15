<?php

include_once("BluePHP/GUI/Button.inc");
include_once("BluePHP/GUI/Concat.inc");
include_once("BluePHP/BlueSystem/GUI/BSApplication.inc");

$button = new Button("bt_hello", array( "label" => "Click me" ));

$app = new BSApplication();

// concat simply put elements one after the other
$frame = new Concat(array($button));

// the root window is always needed
$app->setRootWindow($frame);

// connect the button click to the render method
// of the HelloWorldDialog.inc class located at BluePHP/doc/tutorial/HelloWorldDialog
$app->pushReadyJS($button->eventClick($app->eventRender("doc.tutorial.HelloWorldDialog")));

$app->render();

?>