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
class WorkTimeApplication extends BSApplication
{
  public function __construct($win = null, $requiredParams = null)
  {
    parent::__construct($win, $requiredParams, "BluePHP/BlueProjectPHP/templates/");

    $this->setContent("WorkTimeApplication");

    $this->addLib("BluePHP", array("version" => null, "path" => "/BluePHP/js/"));
    $this->addLib("cir", array("version" => null, "path" => "/CIRGenerator/"));


    $this->addRenderStyle("/BlueProjectPHP/styles/blueeyevideo.css");
    $this->addRenderStyle("/CIRGenerator/report.css");
  }

  public function getReference()
  {
    return array("APPLI" => "CIRGenerator", "VERSION" => "1.0", "REVISION" => "666");
  }
}

?>