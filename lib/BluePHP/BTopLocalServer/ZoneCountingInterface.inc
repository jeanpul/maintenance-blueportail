<?php

include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/BlueSystem/GUI/BSModuleInterface.inc");
include_once("BluePHP/BTopLocalServer/ZoneCounting.inc");

class ZoneCountingInterface extends BSModuleInterface
{
  public function __construct($id = "", $params = array())
  {
    parent::__construct($id, $params);
    parent::setModule(new ZoneCounting());
    parent::setTableParams( array( "titles" => array( _("Id"), _("Area/Location"), _("Type") ),
				   "cols" => array( "id", "nameZ", "zc_type" ) ));
    $this->setCreateLinkTitle(_("Add a new zone counting indicator"));
    $bs = $this->getDialogTable();
    $bs->setTitle(_("Zone counting configuration"));
    parent::setModuleName("BTopLocalServer");
  }

  function getTableInput($params)
  {
    $params = array_merge(array("calendar" => null,
				"zone" => null,
				"zc_id" => null ),
			  $params);
    
    $rows = H();

    if(isset($params["id"]))
      {
	$rows->row( array( _("Id"),
			   H()->input()->withValue($params["id"])
			   ->withType("text")->withMaxLength(8)
			   ->setReadOnly(true)->withName("id"))
		    );
      }

    $rows->row( array( _("Calendar"),
		       createSelect( $this->getModule()->getCalendars(),
				     array( "key" => "id",
					    "name" => "calendar",
					    "value" => "name",
					    "default" => $params["calendar"],
					    "extra" => false ))
		       )
		);

    $rows->row( array( _("Area/Location"),
		       createSelect( $this->getModule()->getZones(),
				     array( "key" => "id",
					    "name" => "zone",
					    "value" => "name",
					    "default" => $params["zone"],
					    "extra" => false ))
		       )
		);

    $rows->row( array( _("Type"),
		       createSelect( $this->getModule()->getFunctions(),
				     array( "key" => "id",
					    "name" => "zc_id",
					    "value" => "name",
					    "default" => $params["zc_id"],
					    "extra" => false ))
		       )
		);   
    
    $table = H()->table($rows);
    $gui = H()->form($table)->withId($this->getClassName() . "EditForm");

    $this->setDialogParams( array( "msg" => $gui->str() ));
    $this->setEditTitle(_("Edit zone counting indicator parameters"));
    $this->setCreateTitle(_("Create a new zone counting indicator"));
    parent::getTableInput($params);
  }

  public function delEntrySubmission()
  {
    $this->setDeleteTitle(_("Delete a Zone Counting indicator"));
    $this->setDialogParams(array( "msg" => H()->p(_("Are you sure to delete this zone counting indicator ?"))->str()));
    parent::delEntrySubmission();
  }
  
}



?>