<?php

include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/BlueSystem/GUI/BSModuleInterface.inc");
include_once("BluePHP/BTopLocalServer/FlowCountingProcessing.inc");

class FlowCountingProcessingInterface extends BSModuleInterface
{
  public function __construct($id = "", $params = array())
  {
    parent::__construct($id, $params);
    parent::setModule(new FlowCountingProcessing());
    parent::setTableParams( array( "titles" => array( _("Id"),_("Door/Group"), _("Type") ),
				   "cols" => array( "id", "nameF", "fc_type" ) ));
    $this->setCreateLinkTitle(_("Add a new Flow counting indicator"));
    $bs = $this->getDialogTable();
    $bs->setTitle(_("Flow counting configuration"));
    parent::setModuleName("BTopLocalServer");
  }

  function getTableInput($params)
  {
    $params = array_merge(array("calendar" => null,
				"flow" => null,
				"fc_id" => null ),
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

    $rows->row( array( _("Door/Group"),
		       createSelect( $this->getModule()->getFlows(),
				     array( "key" => "id",
					    "name" => "flow",
					    "value" => "name",
					    "default" => $params["flow"],
					    "extra" => false ))
		       )
		);

    $rows->row( array( _("Type"),
		       createSelect( $this->getModule()->getFunctions(),
				     array( "key" => "id",
					    "name" => "fc_id",
					    "value" => "name",
					    "default" => $params["fc_id"],
					    "extra" => false ))
		       )
		);

    $table = H()->table($rows);
    $gui = H()->form($table)->withId($this->getClassName() . "EditForm");

    $this->setDialogParams( array( "msg" => $gui->str() ));
    $this->setEditTitle(_("Edit flow counting indicator parameters"));
    $this->setCreateTitle(_("Create a new flow counting indicator"));
    parent::getTableInput($params);
  }

  public function delEntrySubmission()
  {
    $this->setDeleteTitle(_("Delete a flow Counting indicator"));
    $this->setDialogParams(array( "msg" => H()->p(_("Are you sure to delete this flow counting indicator ?"))->str()));
    parent::delEntrySubmission();
  }
}

?>