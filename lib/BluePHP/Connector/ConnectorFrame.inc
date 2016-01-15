<?php

include_once("BluePHP/GUI/Frame.inc");

class ConnectorFrame extends Frame
{
  public function __construct()
  {
    parent::__construct("default", get_class($this), "BluePHP/Connector/templates");
  }
}

?>