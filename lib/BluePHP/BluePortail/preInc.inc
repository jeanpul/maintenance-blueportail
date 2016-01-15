<?php

include_once('Config.inc');

loadBluePHP();

include_once("BluePHP/Utils/Languages.inc");

include_once("CommonOps.inc");

include_once("BluePortailLang.inc");

setLanguage();

// create connexion to the BluePortail data
$plang = new BluePortailLang();

include_once("headerPart.inc");

?>
