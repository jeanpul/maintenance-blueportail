<?php

include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/BlueSystem/GUI/BSModuleInterface.inc");
include_once("BluePHP/GUI/MenuBarLink.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/Dashboards.inc");

/**
 * \class DashboardsInterface
 */
class DashboardsInterface extends BSModuleInterface
{
  public function __construct($id = "", $params = array())
    {
      parent::__construct($id, $params);
      parent::setModule(new Dashboards());
      parent::setTableParams( array( "titles" => array( _("Id"), _("Module"), _("Label"), _("Icon"), _("Layout"), _("Enabled"), _("Position"), _("Default") ),
				     "cols" => array( "id", "module", "label", "icon", "class", "isActive", "position", "isDefault" )
				     ));
      $this->setCreateLinkTitle(_("Create a new dashboard"));
      parent::setModuleName("BlueCountGUI/Dashboards");
    }
  
  public function getTableInput($p)
  {
    $p = array_merge(array("label" => "",
			   "icon" => "numbering",
			   "class" => "default", 
			   "module" => "Dashboards",
			   "isActive" => "true",
			   "position" => 0,
			   "isDefault" => "true",
			   "bgName" => null ),
		     $p);
    
    $rows = H();

    $rows->row( array( _("Default"),
		       H()->input()->withType("checkbox")
		       ->withName("isDefault")->withValue("true")
		       ->isChecked($p["isDefault"] == "true"))
		);       
    
    if(isset($p["id"]))
      {
	$rows->row( array( _("Id"),
			   H()->input()->withValue($p["id"])
			   ->withType("text")->withMaxLength(8)
			   ->setReadOnly(true)->withName("id"))
		    );
      }  
    $rows->row( array( _("Label"),
		       H()->input()->withValue($p["label"])
		       ->withType("text")->withMaxLength(64)
		       ->withName("label"))
		);

    $rows->row( array( _("Theme"),
		       createSelect( array( "default", "smartphone" ),
				     array( "key" => null,
					    "name" => "class",
					    "value" => null,
					    "default" => $p["class"],
					    "extra" => false ))
		       )
		);

    if(is_null($p["bgName"]))
      {
	$bg = $this->getModule()->getDefaultBackground();
	$p["bgName"] = $bg["value"];
      }

    $rows->row( array( _("Background"),
		       createSelect( $this->getModule()->getBackgrounds(),
				     array( "key" => "value",
					    "id" => "bgSelect",
					    "name" => "bgName",
					    "value" => "label",
					    "default" => $p["bgName"],
					    "extra" => "onChange=\"DashEditor.changeBgPreview('bgSelect','bgPreview')\"" ))
		       )
		);

    $rows->tr( H()->td( H()->img()->with('src', 
					 $this->getModule()->getBackgroundURL($p["bgName"]))
			->withId("bgPreview")
			)->with('colspan', '2')
	       );
   
    $table = H()->table($rows);
    $table->input()->withName("isActive")->withValue("true")->withType("hidden");
    $table->input()->withName("position")->withValue($p["position"])->withType("hidden");
    $table->input()->withName("icon")->withValue($p["icon"])->withType("hidden");
    $table->input()->withName("module")->withValue($p["module"])->withType("hidden");
    $gui = H()->form($table)->withId($this->getClassName() . "EditForm");
    
    $this->setDialogParams( array( "msg" => $gui->str() ));
    $this->setEditTitle(_("Edit dashboard parameters"));
    $this->setCreateTitle(_("Create a new dashboard"));
    parent::getTableInput($p);
  }

  protected function getUpdateInterfaceFuncalls($dashboardId)
  {
    $funParams = array( "class" => "Dashboards.DashMainMenu",
			"render" => "mainMenu" );
    if(!is_null($dashboardId))
      {
	$funParams = array_merge( $funParams, array( "params" => "dashboard=" . $dashboardId ) );
      }

    $funcall1 = js_funcall( array( "function" => "BlueSystem.callClassHTML",
				   "options" => $funParams )
			    );

    $funParams = array( "class" => "BTopLocalServer.DashboardEditor",
			"render" => "DashboardPanelsList" );
    if(!is_null($dashboardId))
      {
	$funParams = array_merge( $funParams, array( "params" => "id=" . $dashboardId ) );
      }

    $funcall2 = js_funcall( array( "function" => "BlueSystem.callClassHTML",
				   "options" => $funParams )
			    );

    return $funcall1 . " " . $funcall2;
  }

  public function setEntry()
  {
    $params = $this->getParams();
    $this->getModule()->setEntry($params);
    $cb = "\$(document).ready( function() { DashEditor.switchToDashboard({ 'id': " . $params["id"] . "}); });";
    $render = H()->javascript($cb);
    echo $render;
  }

  public function newEntry($params = null)
  {
    $params = $this->getParams();
    $dashboard = $this->getModule()->newEntry($params);
    $cb = "\$(document).ready( function() { DashEditor.switchToDashboard({ 'id': " . $dashboard["id"] . "}); });";
    $render = H()->javascript($cb);
    echo $render;
  }

  public function delEntry()
  {
    $params = $this->getParams();
    $this->getModule()->delEntry($params);

    $dashboard = $this->getModule()->getDefaultDashboard();
    $dashboardId = null;
    if($dashboard)
      {
	$cb = "\$(document).ready( function() { DashEditor.switchToDashboard({ 'id': " . $dashboard["id"] . "}); });";
      }
    else
      {
	$cb = "\$(document).ready( function() { DashEditor.switchToDashboard(); });";
      }
    $render = H()->javascript($cb);
    echo $render;
  }

  public function delEntrySubmission()
  {
    $this->setDeleteTitle(_("Delete a dashboard"));
    $this->setDialogParams(array( "msg" => H()->p(_("Are you sure to delete this dashboard ?"))->str()));
    parent::delEntrySubmission();
  }

  protected function renderActions($elt, $params)
  {
    $keyparams = parse_str($params["keyparam"]);
    // add viewing action
    $str = getMenuBarLink("", 
			  array( "address" => "qms_conf.dashboards.dashboards_editor", 
				 "params" => "id=" . $elt["id"] ),
			  "<img src='/BTopLocalServer/styles/eye.png'/>");
    $str .= parent::renderActions($elt, $params);
    return $str;
  }
}

?>