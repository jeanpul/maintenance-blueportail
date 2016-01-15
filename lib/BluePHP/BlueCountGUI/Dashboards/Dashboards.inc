<?php

include_once("BluePHP/Utils/CSS.inc");
include_once("BluePHP/Dashboards/Config.inc");
include_once("BluePHP/BlueCountGUI/BCGUI_ModuleProcessing.inc");

class Dashboards extends BCGUI_ModuleProcessing
{
  function __construct($sharedCon = null)
  {
    parent::__construct(array( "name" => "Dashboards",
			       "keyname" => "id",
			       "sort" => "position" ),
			$sharedCon);
  }

  public function getBackgrounds()
  {
    return array( "texture_default_200.png" => array( "value" => "texture_default_200.png",
						   "label" => "Default" ),
		  "texture_wood_200.png" => array( "value" => "texture_wood_200.png",
						   "label" => "Wood" ),
		  "texture_flame_200.png" => array( "value" => "texture_flame_200.png",
						    "label" => "Flame" ),
		  "texture_brick_200.png" => array( "value" => "texture_brick_200.png",
						    "label" => "Brick" ),
		  "texture_marble_200.png" => array( "value" => "texture_marble_200.png",
						     "label" => "Marble" )
		  );
  }

  public function getDefaultBackground()
  {
    return $this->getBackgrounds()["texture_default_200.png"];
  }

  public function setEntry($params)
  {
    $params = array_merge( array( "isActive" => "false",
				  "isDefault" => "false" ),
			   $params );
    $this->generateCSS($params);
    return parent::setEntry($params);
  } 

  public function newEntry($params)
  {
    parent::newEntry($params);
    $dash = $this->getLastDashboard();
    $this->generateCSS($dash);
    return $dash;
  }

  public function getBackgroundURL($bgName)
  {
    return '/Dashboards/styles/' . $bgName;
  }

  protected function generateCSS($params)
  {
    $bgs = $this->getBackgrounds();
    $bgName = $bgs[ $params["bgName"] ]["value"];
    $css = CSS()->body()->with('background-image', "url('" . $this->getBackgroundURL($bgName) . "')")
      ->sel('.DashBoardsPanel')->sel('#Dashboard-' . $params["id"]);
    return file_put_contents(DASHBOARD_STYLE_PATH_BASE . "/dash_" . $params["id"] . ".css", $css->str());
  }

  public function getEntriesFromModule($params)
  {
    return parent::Select("* from Dashboards where module=\"" . $params["module"] . "\"");
  }

  public function getPanelsFromDashboard($params)
  {
    return parent::Select("Panels.*, description as size from Panels, Dashboards, PanelClasses where Panels.idd=Dashboards.id " .
			  "and Dashboards.id=" . $params["idd"] . " and Panels.class=PanelClasses.class " . 
			  "order by position asc");
  }

  public function getPanelsEntry($params)    
  {
    return parent::Select("* from Panels where id=" . $params["id"]);
  }

  public function getLayouts()
  {
    return parent::Select("* from DashboardsClasses");
  }
  
  public function getPanelClasses()
  {
    return parent::Select("* from PanelClasses");
  }

  public function getLayoutFromClass($params)
  {
    return parent::Select("* from DashboardsClasses where class=\"" . $params["class"] . "\"");
  }

  public function getPanelAtPos($params)
  {
    return parent::Select("* from Panels where idd=" . $params["idd"] . " and position=" . $params["position"]);
  }

  public function getLastPosPanel($params)
  {
    return parent::Select("* from Panels where idd=" . $params["idd"] . " order by position desc limit 1");
  }

  public function updatePanelAtPos($params)
  {
    return $params;
  }

  public function getDefaultDashboard($params = array())
  {
    $res = null;
    $elts = parent::Select("* from Dashboards where isDefault='true' limit 1"); 
    if(is_array($elts) and count($elts))
      {
	$res = $elts[0];
      }
    return $res;
  }

  public function getLastDashboard($params = array())
  {
    $res = null;
    $elts = parent::Select("* from Dashboards where id=(select max(id) from Dashboards)"); 
    if(is_array($elts) and count($elts))
      {
	$res = $elts[0];
      }
    return $res;
  }
}

?>