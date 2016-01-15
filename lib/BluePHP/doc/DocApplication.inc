<?php

include_once("BluePHP/JQUERY/JQueryApplication.inc");

/**
 */
class DocApplication extends JQueryApplication
{
  public function __construct($win = null, $requiredParams = null)
  {
    parent::__construct($win, "BluePHP/doc/templates/");
    $this->addLib("BluePHP", array("version" => null, "path" => "/BluePHP/js/"));
    $this->addRenderStyle("/BluePHP/doc/styles/main.css");
  }
}

?>
