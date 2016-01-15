<?php

include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/BlueSystem/GUI/BSModuleInterface.inc");
include_once("BluePHP/BTopLocalServer/NumberingProcessing.inc");

class NumberingProcessingInterface extends BSModuleInterface
{
  public function __construct($id = "", $params = array())
  {
    parent::__construct($id, $params);
    parent::setModule(new NumberingProcessing());
    parent::setTableParams( array( "titles" => array( _("Id"), _("Location/Area"), _("Type") ),
				   "cols" => array( "id", "nameZ", "numb_type" )
				   ));
    $this->setCreateLinkTitle(_("Add a new Numbering Indicator"));
    $bs = $this->getDialogTable();
    $bs->setTitle(_("Numbering configuration"));
    parent::setModuleName("BTopLocalServer");
  }

  public function getTableInput($params)
  {
    $params = array_merge(array("calendar" => null,
				"zone" => null,
				"numb_id" => 0 ),
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

    $rows->row( array( _("Location/Area"),
		       createSelect( $this->getModule()->getAllZones(),
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
					    "name" => "numb_id",
					    "value" => "name",
					    "default" => $params["numb_id"],
					    "extra" => false ))
		       )
		);
    
    $table = H()->table($rows);
    $gui = H()->form($table)->withId($this->getClassName() . "EditForm");
    
    $this->setDialogParams( array( "msg" => $gui->str() ));
    $this->setEditTitle(_("Edit numbering indicator parameters"));
    $this->setCreateTitle(_("Add a new numbering indicator"));
    parent::getTableInput($params);
  }

  public function delEntrySubmission()
  {
    $this->setDeleteTitle(_("Delete a Numbering indicator"));
    $this->setDialogParams(array( "msg" => H()->p(_("Are you sure to delete this numbering indicator ?"))->str()));
    parent::delEntrySubmission();
  }
}

?>