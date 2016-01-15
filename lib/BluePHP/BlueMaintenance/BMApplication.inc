<?php

include_once("BluePHP/BlueSystem/GUI/BSApplication.inc");

class BMApplication extends BSApplication
{
  public function __construct($win = null, $requiredParams = null)
  {
    parent::__construct($win, $requiredParams, "BluePHP/BlueMaintenance/templates");
    $this->addLib("ddsmoothmenu", array("version" => null, "path" => "/JQUERY/"));
    $this->addLib("BlueMaintenance", array("version" => null, "path" => "/BlueMaintenance/js/"));
    $this->addLib("jquery.jclock", array("version" => null, "path" => "/JQUERY/"));
    $this->addRenderStyle("/BlueCountGUI/Dashboards/styles/Dashboards.css");
    $this->addRenderStyle("/BlueCountGUI/Dashboards/styles/DB_720p_layout001.css");
    $this->addRenderStyle("/BlueMaintenance/styles/BlueMaintenance.css"); 
    $this->pushReadyJS(js_funcall(array( "function" => "BlueMaintenance.Init" )));
    $this->pushReadyJS("$(\"#jclock\").jclock({ format: '%A %b %d %H:%M'});");
  }
}

?>