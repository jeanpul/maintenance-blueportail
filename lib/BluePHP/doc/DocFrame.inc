<?php

include_once("BluePHP/GUI/Frame.inc");

class DocFrame extends Frame
{
  public function __construct() 
  {
    parent::__construct("default", get_class($this), "BluePHP/doc/templates");
  }
}

?>
