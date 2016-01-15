<?php

include_once("BluePHP/JQUERY/JQueryApplication.inc");

class DashApplication extends JQueryApplication
{
  public function __construct($panel, $win = null)
  {
    parent::__construct($win, "BluePHP/Dashboards/templates/");
    $this->addRenderStyle("/BlueSystem/styles/jquery-ui-theme.css");
    $this->addRenderStyle("/Dashboards/styles/Dashboards.css");
    $this->addLib("lazyload-min", array("version" => null, "path" => "/JQUERY/"));
    $this->addLib("highcharts", array("version" => null, "path" => "/Highcharts/js/"));
    $this->addLib("BluePHP", array( "version" => null, "path" => "/BluePHP/js/"));
    $this->addLib("BlueSystem", array("version" => null, "path" => "/BlueSystem/js/"));
    $this->addLib("BTopLocalServer", array("version" => null, "path" => "/BTopLocalServer/js/"));
    $this->addLib("Dashboards", array("version" => null, "path" => "/Dashboards/js/"));
    $this->pushReadyJS(js_funcall(array( "function" => "Dashboards.Init",
					 "options" => array( "title" => $panel["label"] ) )));
    $this->pushReadyJS(js_funcall(array( "function" => "Dashboards.startGetValues" )));
  }
}

?>