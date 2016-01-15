<?php

include_once("BluePHP/JQUERY/JQueryApplication.inc");

class DashEditor extends JQueryApplication
{
  private $dashboard = null;

  public function __construct($dashboard, $win = null)
  {
    parent::__construct($win, "BluePHP/Dashboards/templates/");
    $this->dashboard = $dashboard;
    $this->addRenderStyle("/BlueSystem/styles/jquery-ui-theme.css");
    $this->addRenderStyle("/BlueSystem/styles/StyleBlueEyeVideo.css");
    $this->addRenderStyle("/BlueSystem/styles/dataTables.css");
    $this->addRenderStyle("/Dashboards/styles/DashEditor.css");
    $this->addLib("ddsmoothmenu", array("version" => null, "path" => "/JQUERY/"));
    $this->addLib("lazyload-min", array("version" => null, "path" => "/JQUERY/"));
    $this->addLib("highcharts", array("version" => null, "path" => "/Highcharts/js/"));
    $this->addLib("BluePHP", array("version" => null, "path" => "/BluePHP/js/"));
    $this->addLib("BlueSystem", array("version" => null, "path" => "/BlueSystem/js/"));
    $this->addLib("BTopLocalServer", array("version" => null, "path" => "/BTopLocalServer/js/"));
    $this->addLib("DashEditor", array("version" => null, "path" => "/Dashboards/js/"));
    $this->pushReadyJS(js_funcall(array( "function" => "DashEditor.Init",
					 "options" => $dashboard )));
    $this->setContent(get_class($this));
  }

  protected function getDashboard()
  {
    return $this->dashboard;
  }
}

?>