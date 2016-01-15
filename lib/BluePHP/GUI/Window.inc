<?php

include_once("BluePHP/GUI/ComponentTemplate.inc");

/**
 * \class Window
 * Extends the ComponentTemplate to abstract
 * a window component. A window component is a sort of
 * limited Application that have a title attribute,
 * provides some way to get and set arguments and
 * a _head.template file 
 * \warning do not use this class
 * \todo is this class really used ?
 */
class Window extends ComponentTemplate
{
  /**
   * Window title
   */
  private $title = false;
  private $winParams = array();

  /**
   * Create the window with a specific title
   * \param $title window title
   * \param $content template basename
   * \param $basePath template path
   */
  function __construct($title, $content, $basePath = "templates")
    {
      parent::__construct($content, $basePath);
      $this->title = $title;
      $this->basePath = $basePath;
    }

  /**
   * Returns the title
   */
  function getTitle()
  {
    return $this->title;
  }

  /**
   * Retrieve the window parameters
   */
  function getParams()
  {
    return $this->winParams;
  }

  /**
   * Set the window parameters
   * \param $winParams window parameters
   */
  function setParams($winParams)
  {
    $this->winParams = $winParams;
  }

  /**
   * Set a window parameter value
   * \param $name parameter symbolic name
   * \param $value parameter value
   */
  function setParam($name, $value)
  {
    $this->winParams[$name] = $value;
  }

  /**
   * Retrieve a particuler parameter value
   * \param $name parameter name
   */
  function getParam($name)
  {
    return isset($this->winParams[$name]) ? $this->winParams[$name] : false;
  }

  /**
   * Set the window title
   * \param $title window title string
   */
  function setTitle($title)
  {
    $this->title = $title;
  }

  /**
   * Render the head part of the window
   * This use include_once instead of include for
   * the main part
   */
  public function render_head()
  {
    $render_head_filename = $this->generate_file_content("_head.template");
    if(file_exists($render_head_filename))
      {
	include_once($render_head_filename);
      }
  }

}

?>
