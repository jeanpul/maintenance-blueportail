<?php

error_reporting(E_ALL);

include_once("BluePHP/doc/DocApplication.inc");
include_once("BluePHP/doc/MainPage.inc");

$app = new DocApplication(new MainPage());

$app->render();

?>
