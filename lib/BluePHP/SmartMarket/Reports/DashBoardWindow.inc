<?php

require_once("BluePHP/GUI/Window.inc");

class DashBoardWindow extends Window
{
  var $config;

  public function __construct($config, $basePath)
    {      
      parent::__construct(_("Daily Report"), "index", $basePath);
      $this->config = $config;
    }
}

?>

