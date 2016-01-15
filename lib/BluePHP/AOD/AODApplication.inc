<?php

include_once("BluePHP/JQUERY/JQueryApplication.inc");

/**
 */
class AODApplication extends JQueryApplication
{
  public function __construct($win = null, $requiredParams = null)
  {
    parent::__construct($win, "BluePHP/AOD/templates/");
    $this->addRenderStyle("/AOD/styles/aod.css");
  }
}

?>