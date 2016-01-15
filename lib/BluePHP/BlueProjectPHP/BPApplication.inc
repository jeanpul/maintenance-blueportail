<?php

include_once("BluePHP/GUI/MenuBarEntry.inc");
include_once("BluePHP/BlueSystem/GUI/BSApplication.inc");

/**
 * This is the base class that manages
 * the root part of the GUI. This contains
 * shared components (headers, footers, ...)
 * and common dynamic contents such as ref, lang
 * logo...
 */
class BPApplication extends BSApplication
{

  private $navigation = array();
  private $systems = array();

  public function __construct($win = null, $requiredParams = null)
  {
    parent::__construct($win, $requiredParams, "BluePHP/BlueProjectPHP/templates/");
    $this->addLib("BlueProjectPHP", array("version" => null, "path" => "/BlueProjectPHP/js/"));

    $this->addRenderStyle("/BlueProjectPHP/styles/StyleBlueEyeVideo.css");
    $this->addRenderStyle("/BlueCountGUI/Dashboards/styles/Dashboards.css");
    $this->addRenderStyle("/BlueCountGUI/Dashboards/styles/DB_720p_layout001.css"); 
    $this->pushReadyJS(js_funcall(array( "function" => "BlueProjectPHP.Init" )));
  }

  public function getReference()
  {
    return array("APPLI" => "BlueProjectPHP", "VERSION" => "1.0", "REVISION" => "666");
  }
}

?>