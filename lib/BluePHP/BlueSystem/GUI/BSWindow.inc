<?php

include_once("BluePHP/GUI/Window.inc");

class BSWindow extends Window
{
  private $root = null;

  public function __construct($title, $content, $root, $basePath = "templates")
  {
    parent::__construct($title, $content, $basePath);
    $this->root = $root;
  }

  protected function getRoot()
  {
    return $this->root;
  }
}
