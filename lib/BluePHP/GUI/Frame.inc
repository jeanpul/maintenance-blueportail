<?php

include_once("BluePHP/GUI/ComponentTemplate.inc");

/**
 * \class Frame
 * Extends the ComponentTemplate to provide
 * an id to the renderable. In fact this id
 * will be used to set the id attribute
 * of the html tag and used by javascript plugin
 * to retrieve the element rendered
 */
class Frame extends ComponentTemplate
{
  private $id = null;

  /**
   * Create the element with a specific id. The id is composed of
   * the class name and the specified parameter : "classname"_"id"
   * \param $id the id used to identify the displayed html part
   * \param $content the basename of the template
   * \param $basePath optional directory argument to find the template
   * \sa get_class
   */
  public function __construct($id, $content, $basePath = "templates")
  {
    parent::__construct($content, $basePath);
    $this->id = get_class($this) . ($id != "" ? "_" . $id : "");
  }

  /**
   * Retrieve the id of this component
   */
  protected function getId()
  {
    return $this->id;
  }

  protected function setId($id)
  {
    $this->id = $id;
  }

  /**
   * Retrieve the title
   * \todo should find where this method is used and maybe use an interface
   */
  public function getTitle()
  {
    return $this->id;
  }
}

?>
