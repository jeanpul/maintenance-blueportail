<?php

include_once("BluePHP/Window.inc");

class OnSiteWindow extends Window
{
  var $config;

  function OnSiteWindow($config, $basePath)
    {      
      $this->Window(_("Données temps réel sur site"), "index", $basePath);
      $this->config = $config;
    }
}

?>

