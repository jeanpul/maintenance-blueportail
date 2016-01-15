<?php

include_once("BluePHP/GUI/ComponentTemplate.inc");

/**
 * \class Application
 * Base class that manage the different
 * components of the html page. This class
 * is used by the renderable (Component) to
 * retrieve the parameters, to add new javascript
 * library, new css style, dynamic javascript...
 */
class Application extends ComponentTemplate
{
  /** 
   * reference to the main renderable 
   */
  protected $mainWindow = null; 
  private $styles = array();
  private $modules = array();

  /**
   * Initialisation
   * \param $mainWindow the main renderable
   * \param $basePath optional path to locate the template file
   */
  public function __construct($mainWindow = null, $basePath = "templates")
  {
    parent::__construct("Application", $basePath);
    $this->mainWindow = $mainWindow;
  }

  /**
   * Set the main renderable
   */
  public function setRootWindow($w)
  {
    $this->mainWindow = $w;
  }

  /**
   * Add a css style
   * \param $elt contains the path to the css
   */
  public function addRenderStyle($elt)
  {
    $this->styles[] = $elt;
  }

  /**
   * Add or replace a javascript library
   * The included name will be "path"/"name"-"version".js
   * \param $name
   * \param $params
   */
  public function addLib($name, $params)
  {
    $params = array_merge(array( "path" => "",
				 "version" => "" ), $params);
    $this->modules[$name] = $params;
  }

  /**
   * Retrieves the css files to include
   */
  protected function getStyles()
  {
    return $this->styles;
  }

  /**
   * Retrieves the js file to include
   */
  protected function getLibs()
  {
    return array_keys($this->modules);
  }

  /**
   * Create the javascript library name
   */
  protected function getLibFile($name)
  {
    return $this->modules[$name]["path"] . $name . 
      (is_null($this->modules[$name]["version"]) ? "" : 
       "-" . $this->modules[$name]["version"]) . ".js";
  }

  /**
   * Retrieve the main window title used for the title html tag.
   * This use the title for the main renderable or "default" otherwise
   */
  protected function getTitle()
  {
    return $this->mainWindow ? $this->mainWindow->getTitle() : "default";
  }

  /**
   * Render the main window
   */
  protected function renderWindow()
  {
    return $this->mainWindow ? $this->mainWindow->render() : "";
  }

  /**
   * Render the javascript elements
   */
  protected function renderJS()
  {
    return "";
  }
}

?>
