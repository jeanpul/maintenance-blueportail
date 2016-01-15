<?php

include_once("BluePHP/GUI/Application.inc");
include_once("BluePHP/GUI/Window.inc");
include_once("BluePHP/Utils/ModuleProcessing.inc");
include_once("WaitingTimeWindow.inc");

class WaitingTime extends ModuleProcessing {

  function WaitingTime()
  {
    parent::setParams( array( "name" => "WaitingTime",
			      "keyname" => "id",
			      "sort" => "id" ));
  }  

  function process($config)
  {
    $basePath = BS_BASEPATH . "/configs/WaitingTime/" . $config["type_data"]["theme"];
    $app = new Application(new WaitingTimeWindow($config, $basePath), $basePath);
    $app->render();
    return true;
  }

}

?>
