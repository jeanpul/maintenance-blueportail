<?php

require_once("BluePHP/GUI/Window.inc");
require_once("BluePHP/BlueScreen/Config.inc");

class CountingSummaryWindow extends Window
{
  var $config;

  function CountingSummaryWindow($config, $basePath)
    {
      $this->Window(_("CountingSummary display"), "index", $basePath);
      $this->config = $config;
    }
}


?>