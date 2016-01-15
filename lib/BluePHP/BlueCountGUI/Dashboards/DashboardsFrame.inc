<?php

include_once("BluePHP/BlueSystem/GUI/BSFrame.inc");

class DashboardsFrame extends BSFrame
{
  public function __construct($id, $params = null, $root = null, 
			      $location = "BluePHP/BlueCountGUI/Dashboards/templates")
  {
    parent::__construct($id, $params, $root, $location);
  }

  protected function pushLinksElt($elt)
  {
    return parent::pushLinksElt(array_merge( array( "module" => "BlueCountGUI/Dashboards"), $elt));
  }
}

?>