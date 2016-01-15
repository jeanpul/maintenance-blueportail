<?php

error_reporting(E_ALL);

include_once("BluePHP/GUI/ComponentTemplate.inc");
include_once("BluePHP/doc/DocApplication.inc");

$app = new DocApplication(new ComponentTemplate("HowToWriteHTML",
						"BluePHP/doc/howtos/"));

$app->render();

?>
