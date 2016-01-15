<?php

include_once("BluePHP/GUI/ModuleInterface.inc");
include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/Dashboards.inc");

class DashboardEditorHTML extends ModuleInterface
{
  private $module = null;
  private $dashboard = null;

  public function __construct($id = "DashboardEditor", $params = array())
  {
    parent::__construct($id, $params);
    parent::setModule(new Dashboards());
    // instanciate module and retrieve dashboard parameters
    $this->dashboard = $this->getModule()->getEntry( array( "id" => $params["id"] ));
  }

  function getTableInput($params)
  {
    $cssName = "/BlueCountGUI/Dashboards/styles/" . $this->getClass() . ".css";
    $str = '<link rel="stylesheet" type="text/css" href="/BlueCountGUI/Dashboards/styles/Dashboards.css"/>' . 
      '<link rel="stylesheet" type="text/css" href="' . $cssName; . '"/>';
    $str .= '<div class="DashBoardsPanel DashBoardsPanelLayout">' . 
      '<h2>' . $this->getClass() . '</h2>';
    $layout = $this->getLayout();
    for($i = 0; $i < $layout["nbPosition"]; ++$i)
      {
	$p = new PanelLayoutHTML("PanelLayout_" . $i, array( "idd" => $this->getId(),
							     "position" => $i ));
	$p->render();
      }   
  }

  protected function getId()    
  {
    return $this->dashboard["id"];
  }

  protected function getClass()
  {
    return $this->dashboard["class"];
  }

  protected function getPanels()
  {
    $res = array();
    $params = $this->getParams();
    $panels = $this->module->getPanels(array( "id" => $params["id"] ));
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

  protected function getLayout()
  {
    $res = $this->module->getLayoutFromClass( array( "class" => $this->getClass() ) );
    return $res[0];
  }

}

?>