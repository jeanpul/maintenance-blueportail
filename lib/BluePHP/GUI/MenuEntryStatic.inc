<?php

include_once("BluePHP/GUI/Component.inc");

/**
 * \class MenuEntryStatic
 * Simple MenuEntry wich returns
 * the content specified at construction
 * \todo should inherit of a SimpleRender class or something
 */
class MenuEntryStatic implements Component
{
  private $content = null;

  /**
   * Simply set the content of this renderable.
   * \param $content the content to display
   */
  public function __construct($content)
  {
    $this->content = $content;
  }

  /**
   * Returns the content to display
   */
  protected function getContent()
  {
    return $this->content;
  }

  protected function setContent($c)
  {
    $this->content = $c;
  }

  public function render()
  {
    return $this->getContent();
  }
}

?>