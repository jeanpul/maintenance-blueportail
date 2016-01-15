<?php

include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/BlueSystem/GUI/BSModuleInterface.inc");
include_once("BluePHP/BTopLocalServer/WaitingTimeProcessing.inc");
include_once("BluePHP/BTopLocalServer/Flows.inc");

class WaitingTimeProcessingInterface extends BSModuleInterface
{
  public function __construct($id = "", $params = array())
  {
    parent::__construct($id, $params);
    parent::setModule(new WaitingTimeProcessing());
    parent::setTableParams( array( "titles" => array( _("Id"), _("Area/Location"), _("Door/Group"), _("From"), _("To"), _("Type") ),
				   "cols" => array( "id", "nameZ", "nameF", "nameZ1", "nameZ2", "wt_type" ) 
				   ));
    $this->setCreateLinkTitle(_("Add new Waiting Time indicator"));
    $bs = $this->getDialogTable();
    $bs->setTitle(_("Waiting Time configuration"));
    parent::setModuleName("BTopLocalServer");
  }

  function getTableInput($params)
  {
    $params = array_merge(array("calendar" => null,
				"zone" => null,
				"flow" => null,
				"sens" => 1,
				"wt_id" => null ),			  			  
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

    $module = new Flows($this->getModule());
    $flows = $module->getEntries();
    $rows->row( array( _("Door/Group"),
		       createSelect( $flows,
				     array( "key" => "id",
					    "name" => "flow",
					    "value" => "name",
					    "default" => $params["flow"],
					    "extra" => "onChange=\"BTopLocalServer.ModuleChangeDirection('WaitingTimeProcessing','flow')\"" ))));

    if(is_null($params["flow"]))
      {
	$params["flow"] = $flows[0]["id"];
      }

    $rows->row( array( _("Direction"),
		       createSelect( $module->getOrientedEntry( array( "id" => $params["flow"] )),
				     array( "key" => "sens",
					    "name" => "sens",
					    "value" => "nameZ1,nameZ2",
					    "default" => $params["sens"],
					    "sep" => " => " ))));
    
    $rows->row( array( _("Type"),
		       createSelect( $this->getModule()->getFunctions(),
				     array( "key" => "id",
					    "name" => "wt_id",
					    "value" => "name",
					    "default" => $params["wt_id"],
					    "extra" => false ))
		       )
		);
    
    $table = H()->table($rows);
    $gui = H()->form($table)->withId($this->getClassName() . "EditForm");

    $this->setDialogParams( array( "msg" => $gui->str() ));
    $this->setEditTitle(_("Edit waiting time indicator parameters"));
    $this->setCreateTitle(_("Add a new waiting time indicator"));
    parent::getTableInput($params);
  }

  public function delEntrySubmission()
  {
    $this->setDeleteTitle(_("Delete a Waiting Time indicator"));
    $this->setDialogParams(array( "msg" => H()->p(_("Are you sure to delete this indicator ?"))->str()));
    parent::delEntrySubmission();
  }
}

?>