<?php

include_once("BluePHP/GUI/Application.inc");
/**
 * \class JQueryApplication
 * Manage the Application with the minimal jquery plugins : 
 * jquery-ui, jquery-layout, jquery-dataTables
 * Provides also the use of jquery-ui-theme. Use this class
 * if any of your renderable have some jquery utility.
 */
class JQueryApplication extends Application
{
  private $id = null;
  private $libParams = array();
  private $js = "";
  private $jsReady = "";

  /**
   * Application initialisation so support for jquery is available   
   * \see Application
   * \param $mainWindow
   * \param $basePath
   * \param $version optional jquery version value
   */
  public function __construct($mainWindow = null, $basePath = "templates", $version = "latest")
  {
    parent::__construct($mainWindow, $basePath);
    //    $this->addRenderStyle("/JQUERY/jquery-ui-theme.css");
    $this->libParams = array( "path" => "/JQUERY/",
			     "version" => $version );    
    $this->addLib("jquery", $this->libParams);
    $this->addLib("jquery-ui", $this->libParams);
    //    $this->addLib("jquery.layout", $this->libParams);
    $this->addLib("jQueryRotate.min", array("version" => null, "path" => "/JQUERY/"));
    $this->addLib("jquery.dataTables.min", array("version" => null, "path" => "/JQUERY/"));
    $this->addLib("jquery.dataTables.fnReloadAjax", array("version" => null, "path" => "/JQUERY/"));
    $this->addLib("dataTables.fixedHeader.min", array("version" => null, "path" => "/JQUERY/"));
    $this->addLib("jquery.ba-hashchange.min", array("version" => null, "path" => "/JQUERY/"));
    $this->addLib("jquery.menubar", array("version" => null, "path" => "/BluePHP/JQUERY/"));
    $this->addLib("jgaugeroundbev", array("version" => null, "path" => "/BluePHP/JQUERY/"));
  }

  /**
   * Include a new javascript library to the header. The included name
   * will be "path"/"name"-"version".js
   * \param $name name of the library
   * \param $params optional you could specify the path (default /JQUERY/) and the version (default latest)
   */
  public function addLib($name, $params = array())
  {
    parent::addLib($name, array_merge($this->libParams, $params));
  }

  /**
   * Write some javascript into the jquery ready function
   */
  public function pushReadyJS($str)
  {
    $this->jsReady .= $str . "\n";
  }

  /**
   * Rendering the javascript part
   */
  protected function renderJS()
  {
    return $this->js . "\n" . $this->renderReadyJS();
  }

  /**
   * Rendering the javascript part into the jquery ready function
   */
  protected function renderReadyJS()
  {
    return "$(document).ready( function() {\n" . 
      $this->jsReady . "\n });\n";
  }
}

?>