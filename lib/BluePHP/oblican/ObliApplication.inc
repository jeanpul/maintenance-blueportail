<?php

include_once("BluePHP/JQUERY/JQueryApplication.inc");

/**
 */
class ObliApplication extends JQueryApplication
{
  public function __construct($win = null, $requiredParams = null)
  {
    parent::__construct($win, "BluePHP/oblican/templates/");
    $this->addRenderStyle("/oblican/styles/base.css");
    $this->addRenderStyle("/oblican/styles/skeleton.css");
    $this->addRenderStyle("/oblican/styles/layout.css");
  }
}

?>