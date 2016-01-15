<?php

include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/BlueSystem/GUI/BSModuleInterface.inc");
include_once("BluePHP/BTopLocalServer/Groups.inc");
include_once("BluePHP/BTopLocalServer/Zones.inc");

class GroupsInterface extends BSModuleInterface
{
  public function __construct($id = "", $params = array())
  {
    parent::__construct($id, $params);
    parent::setModule(new Groups());
    parent::setTableParams( array( "titles" => array( _("Id"), _("Name"), _("From"), _("To") ),
				   "cols" => array( "id", "name", "nameZ1", "nameZ2" )
				   ));
    $this->setCreateLinkTitle(_("Create a new Group"));
    $bs = $this->getDialogTable();
    $bs->setTitle(_("Groups configuration"));
    parent::setModuleName("BTopLocalServer");
  }    

  public function getTableInput($params)
  {
    $params = array_merge(array("name" => ""), $params);
    
    $rows = H();

    if(isset($params["id"]))
      {
	$rows->row( array( _("Id"),
			   H()->input()->withValue($params["id"])
			   ->withType("text")->withMaxLength(8)
			   ->setReadOnly(true)->withName("id"))
		    );
      }

    $rows->row( array( _("Name"),
		       H()->input()->withValue($params["name"])
		       ->withType("text")->withMaxLength(64)
		       ->withName("name"))
		);

    $module = new Zones($this->getModule());    
    $locations = $module->getEntries();
    $rows->row( array( _("From"),
		       createSelect($locations,
				    array( "value" => "name",
					   "name" => "zone1",
					   "key" => "id",
					   "default" => isset($params["zone1"]) ? $params["zone1"] : null )))
		);

    $rows->row( array( _("To"),
		       createSelect($locations,
				    array( "value" => "name",
					   "name" => "zone2",
					   "key" => "id",
					   "default" => isset($params["zone2"]) ? $params["zone2"] : null )))
		);

    $table = H()->table($rows);
    $table->input()->withType("hidden")->withName("virtual")->withValue(0);

    $gui = H()->form($table)->withId($this->getClassName() . "EditForm");

    $this->setDialogParams( array( "msg" => $gui->str() ));
    $this->setEditTitle(_("Edit group parameters"));
    $this->setCreateTitle(_("Create a new group"));
    parent::getTableInput($params);
  }

  public function delEntrySubmission()
  {
    $params = $this->getParams();
    $elt = $this->getModule()->getEntry( array( "id" => $params["id"] ) );
    $name = $elt["name"];
    $this->setDeleteTitle(_("Delete a group"));
    $this->setDialogParams(array( "msg" => H()->p(_("Are you sure to delete the group $name ?"))->str()));
    parent::delEntrySubmission();
  }
}

?>