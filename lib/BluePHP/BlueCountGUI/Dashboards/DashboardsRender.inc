<?php

include_once("BluePHP/BlueCountGUI/Dashboards/DashboardsFrame.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/Dashboards.inc");

class DashboardsRender extends DashboardsFrame
{
  private $module = null;
  private $dashboard = null;

  public function __construct($id, $params)
    {
      parent::__construct($id, $params);
      // instanciate module and retrieve dashboard parameters
      $this->module = new Dashboards(); 
      $this->dashboard = $this->module->getEntry( array( "id" => $params["id"] ));
    }

  protected function getClass()
  {
    return $this->dashboard["class"];
  }

  protected function getId()
  {
    return $this->dashboard["id"];
  }

  public function getPanels()
  {
    $res = array();
    $params = $this->getParams();
    $panels = $this->module->getPanelsFromDashboard(array( "idd" => $params["id"] ));
    foreach($panels as $p)
      {
	if($p["isActive"] == "true")
	  {	    
	    include_once("BluePHP/BlueCountGUI/Dashboards/" . $p["class"] . "HTML.inc");
	    $className = $p["class"] . "HTML";
	    $res[] = new $className($p["class"], array( "id" => $p["id"] ));
	  }
      }
    return $res;
  }
}

?>