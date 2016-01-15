<?php

include_once("BluePHP/Utils/js_lang.inc");
include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/DashboardsFrame.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/Panels.inc");

class PanelLayoutHTML extends DashboardsFrame
{
  private $panel = null;

  public function __construct($id, $params, $panel = null)
  {
    parent::__construct($id, $params);
    if($panel) 
      {
	$this->panel = $panel;
      } 
    else
      {
	$module = new Panels();
	$p = $module->getPanelAtPos( array( "idd" => $params["idd"], "position" => $params["position"] ) );
	if(is_array($p) and count($p)) 
	  {	    
	    include_once("BluePHP/BlueCountGUI/Dashboards/" . $p[0]["class"] . "HTML.inc");
	    $className = $p[0]["class"] . "HTML";
	    $this->panel = new $className($p[0]["class"], array( "id" => $p[0]["id"] ));
	  }
	else
	  {
	    $this->panel = null;
	  }
      }
  }

  public function getPanel()
  {
    return $this->panel;
  }

  public function renderPanelEdit()
  {
    $this->panel->renderPanelEdit();
  }

}

?>