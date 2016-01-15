<?php

include_once("BluePHP/Utils/HTML.inc");

echo (new HTML())->p("Lorem ipsum dolor sit amet, consectetur adipiscing elit.")->withClass("warning")->form()->withId("InstallationForm")->str();

?>