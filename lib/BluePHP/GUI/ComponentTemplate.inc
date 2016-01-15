<?php

include_once("BluePHP/GUI/Component.inc");

/**
 * \class ComponentTemplate
 * This is the base class for any renderable (which means that 
 * something will be displayed when the method render is call) that
 * use template file. A template file (with the .template filename extension),
 * is a file that will be displayed using the php include directive when
 * the render method is called (by the Application class or by containers)
 */
class ComponentTemplate implements Component
{
  private $content = null;
  private $basePath = "templates";

  /**
   * Simply indicate where to find the template file :
   * $basePath/$content.template
   * \param $content the basename of the template
   * \param $basePath optional directory argument to find the template
   */
  public function __construct($content, $basePath = "templates")
  {
    $this->content = $content;
    $this->basePath = $basePath;
  }

  /**
   * Rendering method using the php include directive.
   */
  public function render()
  {
    include($this->generate_file_content(".template"));
  }

  public function renderAsStr()
  {
    ob_start();
    $this->render();
    return ob_get_clean();
  }

  /**
   * Create the path to the template filename
   * \param $suffix the template suffix (i.e ".template")
   */
  protected function generate_file_content($suffix)
  {
    return $this->basePath . "/" . $this->content . $suffix;
  }

  /**
   * Set the basename of the template file
   * \param $content basename
   */
  public function setContent($content)
  {
    $this->content = $content;
  }

  /**
   * Set the path of the template location
   */
  public function setBasePath($basePath)
  {
    $this->basePath = $basePath;
  }
}

?>