<?php

error_reporting(E_ALL);

include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/GUI/Concat.inc");
include_once("BluePHP/GUI/Dialog.inc");
include_once("BluePHP/BlueSystem/GUI/BSApplication.inc");

$elts = array();

foreach(glob("../tutorial/*.php") as $filename) 
  {
    $id = basename($filename, ".php");
    $d = new Dialog($id, $id . ".php", null);
    $d->setHeader( createLink( array( "url" => $filename,
				      "value" => $id . ".php",
				      "nofollow" => true )));
    $d->setContent(highlight_file($filename, true));
    array_push($elts, $d);
  }

$app = new BSApplication(new Concat($elts));
$app->addRenderStyle("/BluePHP/doc/styles/main.css");
$app->render();

?>