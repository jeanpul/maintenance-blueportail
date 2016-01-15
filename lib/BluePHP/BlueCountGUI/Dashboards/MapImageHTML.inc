<?php

include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/Dashboards/Config.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/PanelHTML.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/MapImageConfig.inc");

class MapImageHTML extends PanelHTML
{
  public function __construct($id, $params)
  {
    parent::__construct($id, $params);
  }

  public function getPostContent()
  {
    $module = new MapImageConfig();

    $params = array_merge( $module->getDefaultParams(),
			   $this->getPanel() );

    $content = H();
    if($params["id"] != "null")
      {
	$content = H();

	if($params["src"] != "")
	  {
	    $content->h4("Preview");
	    $content->img()->withSrc($this->getImageLink())
	      ->withId("MapImageImg_" . $params["id"])
	      ->withStyle("width: 400px;");
	  }
	else
	  {
	    $content->h4("No image uploaded");
	    $content->img()->withId("MapImageImg_" . $params["id"])
	      ->withStyle("width: 600px;");
	  }


	$content->form( H()->input()->withType("file")->withName("upfile")
		    ->withId("uploader")->withOnChange("DashEditor.callImageUploader();")
		    ->input()->withType("hidden")->withName("id")->withValue($params["id"])		    
		    )->withId("MapImageUploaderForm")->withEncType("multipart/form-data")
	  ->withAction("/Dashboards/Editor/MapImageUploader.php")
	  ->withMethod("POST")->withTarget("MapImageUploaderTarget");

	$content->iframe()->withId("MapImageUploaderTarget")->withName("MapImageUploaderTarget")
	  ->withStyle("width:0;height:0;border:0px;");
      }
    return $content;
  }

  public function getFormContent()
  {
    $module = new MapImageConfig();

    $params = array_merge( $module->getDefaultParams(),
			   $this->getPanel() );
    if($params["id"] != "null") 
      {
	$rows = H();	
	$rows->input()->withType("hidden")->withName("src")
	  ->withId("MapImageSrc")
	  ->withValue("map_" . $params["id"] . ".jpg");
	$rows->input()->withType("hidden")->withName("alt")->withValue("Map");
	$content = parent::getFormContent()->table($rows);
      }
    else
      {
	$content = parent::getFormContent()
	  ->p("You can upload your map image only by editing the widget when it is added to the dashboard.")
	  ->withClass("info");
      }

    return $content;
  }

  protected function getImageLink()
  {
    $p = $this->getPanel();
    return DASHBOARD_STYLE_URL_BASE . $p["src"];
  }
}

?>