<?php


include_once("BluePHP/GUI/Application.inc");
include_once("BluePHP/GUI/Window.inc");
include_once("BluePHP/Utils/ModuleProcessing.inc");
include_once("CountingSummaryWindow.inc");


class CountingSummary extends ModuleProcessing {

  function CountingSummary()
  {
    parent::setParams( array( "name" => "CountingSummary",
			      "keyname" => "id",
			      "sort" => "id" ));
  }  

  function process($config)
  {
    $basePath = BS_BASEPATH . "/configs/CountingSummary/" . $config["type_data"]["theme"];
    $app = new Application(new CountingSummaryWindow($config, $basePath), $basePath);
    $app->render();
    return true;
  }

}

?>
