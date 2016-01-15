<?php

include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/GUI/Frame.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/DashboardsFrame.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/Dashboards.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/Panels.inc");

class PanelHTML extends Frame
{
  private $panel = null;

  public function __construct($id, $params, $location = "BluePHP/BlueCountGUI/Dashboards/templates")
  {
    parent::__construct($id, get_class($this), $location);

    if(isset($params["id"]))
      {
	$module = new Panels();
	$this->panel = $module->getEntry(array( "id" => $params["id"] ));
      }
    else
      {
	$this->panel = $params;
      }
  }

  public function getClass()
  {
    return $this->panel["class"];
  }

  public function getPosition()
  {
    return $this->panel["position"];
  }

  public function getId()
  {
    return $this->panel["id"];
  }

  public function getPanel()
  {
    return $this->panel;
  }

  public function isActive()
  {
    return $this->panel["isActive"];
  }

  public function getMaxPos()
  {
    return $this->panel["maxPos"];
  }

  public function getDashboard()
  {
    return $this->panel["idd"];
  }
  
  /**
   * Helper methods for the rendering
   */
  protected function getJSRef()
  {
    return "/Dashboards/js/" . $this->getClass() . ".js";
  }

  protected function getCSSRef()
  {
    $cssName = "/Dashboards/styles/themes/default/" . $this->getClass() . ".css";
    $themeFile = "/Dashboards/styles/themes/" . $this->panel["theme"] . "/" . $this->getclass() . ".css";
    if(file_exists("/var/www/" . $themeFile))
      {
	$cssName = $themeFile;
      }
    return $cssName;
  }

  public function getMediaContent()
  {
    $str = "";
    $filePath = "/var/www/Dashboards/media/" . $this->getClass() . ".html";
    if(file_exists($filePath))
      {
	$str = file_get_contents($filePath);
      }
    return $str;
  }

  protected function getStyleClasses()
  {
    return $this->getClass() . "Panel Panel " . $this->panel["size"] . " Panel-" . $this->panel["category"];
  }

  protected function getStyleId()
  {
    return $this->getClass() . "Panel_" . $this->getId();
  }

  /**
   * must be overloaded to display 
   * extra functionalites outside the form parameters
   * of the widget
   */
  public function getPostContent()
  {
    return H();
  }

  /**
   * must be overloaded in order to provides specific parameters
   */
  public function getFormContent()
  {
    $params = array("isActive" => "true",
		    "category" => null,
		    "class" => null,
		    "position" => 0 );

    if($this->getPanel())
      {
	$params = array_merge( $params, $this->getPanel() );
      }

    $module = new Dashboards();
    $classes = $module->getPanelClasses();
    if($params["class"] == null) {
      $params["class"] = $classes[0]["class"];
    }

    $module = new Panels();
    $categories = $module->getCategories();
    if($params["category"] == null)
      {
	$params["category"] = $categories[0];
      }

    $content = H();
    if(isset($params["id"]))
      {
	$content->h2(_("Widget") . " " . $params["id"]);
      }

    $str = "<ul id=\"WidgetMedia\" class=\"media-list\">\n";    
    $str .= $this->getMediaContent();
    $str .= "</ul>\n";   
    $content->div($str);

    $rows = H();
	
    $module = new Panels();
    $rows->row( array( _("Category color"),
		       createSelect( $module->getCategories(),
				     array( "name" => "category",
					    "default" => $params["category"],
					    "extra" => false ))
		       )
		);
    $content->table($rows);

    if(isset($params["id"]))
      {
	$content->input()->withValue($params["id"])->withType("hidden")
	  ->withName("id");
      }
    $content->input()->withValue($params["isActive"])->withType("hidden")
      ->withName("isActive");
    $content->input()->withValue($params["class"])->withType("hidden")
      ->withName("class")->withId("PanelClass");
    $content->input()->withValue($params["position"])->withType("hidden")
      ->withName("position");
    $content->input()->withValue($params["idd"])->withType("hidden")
      ->withName("idd");

    return $content;
  }
}

?>