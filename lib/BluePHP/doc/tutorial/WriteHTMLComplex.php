<?php

include_once("BluePHP/Utils/HTML.inc");

// create the formular content
$form = (new HTML)->table( array( (new HTML)->row(array( "User", (new HTML)->input()->withType("text"))),   
				  (new HTML)->row(array( "Password", (new HTML)->input()->withType("password")))
				  ))->input()->withType("submit");

echo (new HTML())->p("Lorem ipsum dolor sit amet, consectetur adipiscing elit.")->withClass("warning")->form($form)->withId("InstallationForm");

?>