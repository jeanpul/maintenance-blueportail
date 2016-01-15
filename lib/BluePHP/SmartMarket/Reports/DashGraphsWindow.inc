<?php

include_once("BluePHP/GUI/Window.inc");

class DashGraphsWindow extends Window
{
  var $config;

  public function __construct($config, $basePath)
    {      
      parent::__construct(_("Waiting Time estimation list"), "index", $basePath);
      $this->config = $config;
    }
}

?>
