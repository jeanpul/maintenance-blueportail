<?php

require_once("BluePHP/GUI/Window.inc");
require_once("BluePHP/BlueScreen/Config.inc");

class WaitingTimeWindow extends Window
{
  var $config;

  function WaitingTimeWindow($config, $basePath)
    {
      $this->Window(_("WaitingTime display"), "index", $basePath);
      $this->config = $config;
    }
}

?>
