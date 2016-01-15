<?php

include_once("BluePHP/BlueSystem/GUI/BSApplication.inc");

class QMSApplication extends BSApplication
{
  public function __construct($win = null, $requiredParams = null)
  {
    parent::__construct($win, $requiredParams, "BluePHP/BTopLocalServer/templates/");
    $this->addLib("BTopLocalServer", array("version" => null, "path" => "/BTopLocalServer/js/"));
    $this->addRenderStyle("/BTopLocalServer/styles/blueeyevideo.css");
    //    $this->addRenderStyle("/BlueCountGUI/Dashboards/styles/Dashboards.css");
    //    $this->addRenderStyle("/BlueCountGUI/Dashboards/styles/DB_720p_layout001.css"); 
    $this->pushReadyJS(js_funcall(array( "function" => "BTopLocalServer.Init" )));
  }
}

?>