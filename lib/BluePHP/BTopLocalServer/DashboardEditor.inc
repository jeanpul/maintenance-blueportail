<?php

include_once("BluePHP/Utils/js_lang.inc");
include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/BTopLocalServer/BCMFrame.inc");
include_once("BluePHP/BlueSystem/GUI/BSModuleInterface.inc");
include_once("BluePHP/BlueSystem/GUI/BSDialogSubmit.inc");
include_once("BluePHP/BlueSystem/GUI/BSApplication.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/Dashboards.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/Panels.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/PanelHTML.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/PanelLayoutHTML.inc");

class DashboardEditor extends BSModuleInterface
{
  private $dashboard = null;

  public function __construct($id, $params)
  {
    parent::__construct($id, $params);
    parent::setModule(new Dashboards());
    $this->setContent("DashboardEditor");
    $this->setBasePath("BluePHP/BTopLocalServer/templates");
    $this->setClassName(get_class($this));
    $this->setModuleName("BTopLocalServer");
    if(isset($params["idd"]))
      { // TODO : must always use idd instead of id
	$params["id"] = $params["idd"];
      }
    if(isset($params["id"]) and !is_null($params["id"]))
      {
	$this->dashboard = $this->getModule()->getEntry( array( "id" => $params["id"] ));
      }
    $de = $this->getDialogEdit();
    $de->setWidth("600");
  }

  public function getCurrentDashboard()
  {
    return $this->dashboard;
  }

  public function setEntry()
  {
    $panelModule = new Panels();
    $panelModule->setPanelAtPos($this->getParams());
    $funcall = js_funcall( array( "function" => "BlueSystem.callClassHTML",
				  "options" => array( "class" => "BTopLocalServer.DashboardEditor",
						      "render" => "DashboardPanelsList",
						      "params" => "id=" . $this->getId() )));
    $cb = "\$(document).ready( function() { " . $funcall . "} );";
    $render = H()->javascript($cb);
    echo $render;
  }

  public function newEntry($params = null)
  {    
    $params = $this->getParams();
    $panelModule = new Panels();
    $newPanel = $panelModule->addPanelAtPos($params);
    $funcall = js_funcall( array( "function" => "BlueSystem.callClassHTML",
				  "options" => array( "class" => "BTopLocalServer.DashboardEditor",
						      "render" => "DashboardPanelsList",
						      "params" => "id=" . $this->getId() )));
    $cb = "\$(document).ready( function() { " . $funcall . "} );";
    $render = H()->javascript($cb);
    echo $render;
  }

  public function delEntry()
  {
    $panelModule = new Panels();
    $panelModule->removePanelAtPos($this->getParams());
    $funcall = js_funcall( array( "function" => "BlueSystem.callClassHTML",
				  "options" => array( "class" => "BTopLocalServer.DashboardEditor",
						      "render" => "DashboardPanelsList",
						      "params" => "id=" . $this->getId() )));
    $cb = "\$(document).ready( function() { " . $funcall . "} );";
    $render = H()->javascript($cb);
    echo $render;
  }

  public function delEntrySubmission()
  {
    $this->setDeleteTitle(_("Delete a widget"));
    $this->setDialogParams(array( "msg" => H()->p(_("Are you sure to delete this widget ?"))->str()));

    $p = $this->getParams();    
    $keyparam = "id=" . $p["id"] . "&idd=" . $p["idd"] . "&position=" . $p["position"];
    $this->getDialogEdit()->setTitle($this->getDeleteTitle());
    $this->getDialogEdit()->eventSubmit(BS_eventRenderWithParams($this->getModuleName() . "." . 
								 $this->getClassName(),
								 "", 
								 $keyparam,
								 "delEntry"));
    $this->getDialogEdit()->render();
  }

  protected function getId()    
  {
    return $this->dashboard["id"];
  }

  protected function getClass()
  {
    return $this->dashboard["class"];
  }

  protected function getPanels($onlyActive = true)
  {
    $res = array();
    $params = $this->getParams();
    $panels = $this->getModule()->getPanelsFromDashboard(array( "idd" => $params["id"] ));
    foreach($panels as $p)
      {
	if(!$onlyActive or ($onlyActive and $p["isActive"] == "true"))
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
    $res = $this->getModule()->getLayoutFromClass( array( "class" => $this->getClass() ) );
    return $res[0];
  }

  /**
   * Render the Dashboard editor with the panel layout
   * used to add/edit and remove panels on the current dashboard.
   */
  public function renderLayouts()    
  {
    $layout = $this->getLayout();
    $panels = $this->getModule()->getPanelsFromDashboard(array( "idd" => $this->getId() ));
    $maxPosition = 0;
    foreach($panels as $p)
      {	
	$pl = new PanelLayoutHTML($p["position"], array( "idd" => $this->getId(),
							 "position" => $p["position"],
							 "size" => $p["size"] ));
	echo "<div id=\"position-" . $p["position"] . "\">";
	$pl->render();
	echo "</div>";
	$maxPosition = $p["position"] + 1;
      }
    $pl = new PanelLayoutHTML($maxPosition, array( "idd" => $this->getId(),
						   "position" => $maxPosition ));
    $pl->render();
  }

  public function getTableInput($params)
  {
    $params = array_merge( array( "class" => null ),
			   $params );
    $gui = H();
    if($params["action"] == "edit")
      {
	include_once("BluePHP/BlueCountGUI/Dashboards/" . $params["class"] . "HTML.inc");
	$className = $params["class"] . "HTML";
	$panel = new $className($params["class"], array( "id" => $params["idp"] ));
	$gui = $panel->getFormContent();
	$this->setEditTitle(_("Edit widget parameters"));
	$gui = H()->form( $gui )->withId($this->getClassName() . "EditForm"); 
	$post = $panel->getPostContent();
	$this->setDialogParams( array( "msg" => $gui->str() . $post->str() ));	
	parent::getTableInput($params);
      }
    else
      {
	$module = new Dashboards();
	$classes = $module->getPanelClasses();
	if($params["class"] == null) {
	  $params["class"] = $classes[0]["class"];
	}	
	$defaultSelectedClass = $params["class"];
	$str = "<ul id=\"WidgetMedia\" class=\"media-list\">\n";    
	foreach($classes as $k)
	  {
	    $panel = new PanelHTML(null, $k);
	    $str .= $panel->getMediaContent();
	  }
	$str .= "</ul>\n";   	
	$str .= "<script> $(document).ready( function() { DashEditor.createMediaList(\"$defaultSelectedClass\"); } ); </script>";
	$gui->div($str);
	$gui->input()->withValue($params["class"])->withType("hidden")
	  ->withName("class")->withId("PanelClass");
	$gui->input()->withValue($params["idd"])->withType("hidden")
	  ->withName("idd");    
	$gui->input()->withValue($params["position"])->withType("hidden")
	  ->withName("position");    
	$gui = H()->form( $gui )->withId($this->getClassName() . "EditForm"); 
	$this->setDialogParams( array( "msg" => $gui->str() ));	
	$bs = $this->getDialogEdit();
	$bs->setTitle("Add a Widget to the dashboard");
	$bs->eventSubmit(BS_eventRenderWithForm($this->getModuleName() . "." . $this->getClassName(), 
						$this->getId(),
						$this->getClassName() . "EditForm",
						"addWidget"));
	$bs->render();
      }

  }


  public function addWidget()
  {
    $params = $this->getParams();

    // Fab: check that file exist ; bug during media items selection where
    // params[class] is empty
    $classFile = "BluePHP/BlueCountGUI/Dashboards/" . $params["class"] . "HTML.inc";
    if(file_exists("/var/www/" . $classFile))
      {
	include_once($classFile);
	$className = $params["class"] . "HTML";
	$panel = new $className($params["class"], array( "idd" => $params["idd"],
							 "position" => $params["position"],
							 "class" => $params["class"] ));
	$gui = H()->form( $panel->getFormContent() )->withId($this->getClassName() . "AddForm");
	$bs = new BSDialogSubmit($this->getClassName() . "AddDialog",
				 array( "msg" => $gui->str() ));
	$bs->eventSubmit(BS_eventRenderWithForm($this->getModuleName() . "." . $this->getClassName(), 
						$this->getClassName(),
						$this->getClassName() . "AddForm",
						"newEntry"));
	$bs->setTitle(_("Add a widget"));
	$bs->render();
      }
  }

  public function renderAddPanel()    
  {
    $str = "<form>";
    $str .= $this->getTableInput($this->getParams());
    $str .= "</form>\n";
    echo $str;
  }
}

?>