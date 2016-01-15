<?php

include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/BlueSystem/GUI/BSModuleInterface.inc");
include_once("BluePHP/BTopLocalServer/Location.inc");

class LocationsInterface extends BSModuleInterface
{
  public function __construct($id = "", $params = array())
  {
    parent::__construct($id, $params);
    parent::setModule(new Location());
    parent::setTableParams( array("titles" => array( _("Id"), _("Name") ),
				  "cols" => array( "id", "name" )
				  ));
    $this->setCreateLinkTitle(_("Create a new Location"));

    $bs = $this->getDialogTable();
    $bs->setTitle(_("Locations configuration"));

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

    $table = H()->table($rows);
    $table->input()->withType("hidden")->withName("virtual")->withValue(0);

    $gui = H()->form($table)->withId($this->getClassName() . "EditForm");

    $this->setDialogParams( array( "msg" => $gui->str() ));
    $this->setEditTitle(_("Edit Location parameters"));
    $this->setCreateTitle(_("Create a new location"));
    parent::getTableInput($params);
  }

  public function delEntrySubmission()
  {
    $params = $this->getParams();
    $loc = $this->getModule()->getEntry( array( "id" => $params["id"] ) );
    $locName = $loc["name"];
    $this->setDeleteTitle(_("Remove a Location"));
    $this->setDialogParams(array( "msg" => H()->p(_("Are you sure to remove the location $locName ?"))->str()));
    parent::delEntrySubmission();
  }
}

?>