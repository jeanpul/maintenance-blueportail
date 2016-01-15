<?php

include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/BlueSystem/GUI/BSModuleInterface.inc");
include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/GUI/MenuBarLink.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/Dashboards.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/ContainerPanels.inc");

/**
 * \class TabContainerInterface
 */
class TabContainerInterface extends BSModuleInterface
{
  public function __construct($id = "", $params = array())
  {
    parent::__construct($id, $params);
    parent::setModule(new ContainerPanels());
    parent::setTableParams( array( "titles" => array( _("Dashboard"), _("Label"), _("Position") ),
				   "cols" => array( "dlabel", "label", "position" )
				   ));
    $this->setCreateLinkTitle(_("Add a new dashboard"));

    $bs = $this->getDialogTable();
    $bs->setTitle(_("Edit contained dashboards"));

    parent::setModuleName("BlueCountGUI/Dashboards");
  }

  protected function getCreateLinksParams() 
  {
    return parent::getCreateLinksParams() . "&id=" . $this->getParamValue("id") . "&src=" . $this->getParamValue("src");
  }
  
  public function getTableInput($p)
  {
    $p = array_merge(array( "idd" => null, 
			    "label" => "",
			    "position" => 0 ),
		     $p);
    
    $rows = H();

    if(!isset($p["idd"]))
      {
        $p["position"] = $this->getModule()->getNextPos( array("id" => $p["id"]) );
      }

    $rows->row( array( _("Id"),
		       H()->input()->withValue($p["id"])
		       ->withType("text")->withMaxLength(8)
		       ->setReadOnly(true)->withName("id"))
		);

    $disabled = "";    
    $module = new Dashboards();
    $dashboards = $module->getEntriesFromModule( array( "module" => "Dashboards" ) );

    if(!isset($p["idd"]))
      {
	// run through the dashboard list to remove
	// the current one in order to avoid recursive self contained dashboard	
	$found = false;
	for($i = 0; !$found and $i < count($dashboards); ++$i)
	  {
	    if($dashboards[$i]["id"] == $this->getParamValue("src"))
	      {
		unset($dashboards[$i]);
		$found = true;
	      }
	  }
	
	$rows->row( array( _("Dashboard"),
			   createSelect( $dashboards,
					 array( "key" => "id",
						"name" => "idd",
						"value" => "label",
						"default" => $p["idd"],
						"extra" => false ))
			   )
		    );
      }
    else
      {
	$rows->row( array( _("Idd"),
			   H()->input()->withValue($p["idd"])
			   ->withType("text")->withMaxLength(8)
			   ->setReadOnly(true)->withName("idd"))
		    );	
      }

    $rows->row( array( _("Label"),
		       H()->input()->withValue($p["label"])
		       ->withType("text")->withMaxLength(64)
		       ->withName("label"))
		);
    
    $rows->row( array( _("Position"),
		       H()->input()->withValue($p["position"])
		       ->setReadOnly(true)
		       ->withType("text")->withMaxLength(4)
		       ->withName("position"))
		);

    $table = H()->table($rows);
    $gui = H()->form($table)->withId($this->getClassName() . "EditForm");

    $this->setDialogParams( array( "msg" => $gui->str() ));
    $this->setEditTitle(_("Edit dashboard parameters"));
    $this->setCreateTitle(_("Add a new dashboard"));
    parent::getTableInput($p);   
  }

  protected function getModuleEntries()
  {
    return $this->getModule()->getEntriesForPanel(array( "id" => $this->getParamValue("id") ));
  }

  public function delEntrySubmission()
  {
    $this->setDeleteTitle(_("Remove a dashboard"));
    $this->setDialogParams(array( "msg" => H()->p(_("Are you sure to remove this dashboard ?"))->str()));
    parent::delEntrySubmission();
  }

  public function getTableListJSON($p = null)
  {
    if($p != null)
      {
	$this->setParamValue("id", $p["id"]);
      }
    return parent::getModuleTableListJSON($this->getTableParams());
  }

  // fab: MUST CHANGE THIS ; getTableListJSON must
  // handle arguments !!!!
  public function getTableList($pagename = "")
  {
    $bs = new BSDataTable($this->getClassName() . "Table");
    $bs->setContent(parent::getModuleTableList($this->getTableParams(), $pagename));
    $bs->setAjaxSource("/BluePHP/BlueSystem/CallModuleFunction.php?" . 
		       "__module=" . $this->getModuleName() . "&" . 
		       "__class=" . $this->getClassName() . "&" . 
		       "__function=getTableListJSON&" . 
		       "__output=JSON_dataTables&id=" . $this->getParamValue("id"));
    $bsTable = $this->getDialogTable();
    $str = H()->div($bs->renderAsStr())->withClass("BSModuleInterface");
    $bsTable->setMsg($str);
    $bsTable->render();
  }

}

?>