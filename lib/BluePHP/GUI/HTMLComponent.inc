<?php

include_once("BluePHP/GUI/Component.inc");

class HTMLComponent implements Component
{
  private $data = null;

  public function __construct($data)
  {
    $this->data = $data;
  }

  public function render()
  {
    echo $this->data;
  }
}