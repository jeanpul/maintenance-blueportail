<?php

include_once("BluePHP/JQUERY/JQueryApplication.inc");

/**
 */
class ConnectorApplication extends JQueryApplication
{
  public function __construct($win = null, $requiredParams = null)
  {
    parent::__construct($win, "BluePHP/Connector/templates/");
    $this->addLib("BluePHP", array("version" => null, "path" => "/BluePHP/js/"));
    $this->addRenderStyle("/Connector/styles/main.css");
  }
}

?>