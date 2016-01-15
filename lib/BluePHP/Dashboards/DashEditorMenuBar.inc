<?php

include_once("BluePHP/GUI/Frame.inc");

class DashEditorMenuBar extends Frame
{
  public function __construct($id)
  {
    parent::__construct($id, get_class($this), "BluePHP/Dashboards/templates");
  }
}

?>