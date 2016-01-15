<?php

include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/BlueSystem/GUI/BSModuleInterface.inc");
include_once("BluePHP/BTopLocalServer/BTopSensors.inc");
include_once("BluePHP/BTopLocalServer/Doors.inc");

class BTopSensorsInterface extends BSModuleInterface
{
  public function __construct($id = "", $params = array())
  {
    parent::__construct($id, $params);
    parent::setModule(new BTopSensors());
    parent::setTableParams( array( "titles" => array( "id", "Ref", "host", "Door", "From", 
						      "To" ),
				   "cols" => array( "id", "ref", "host", "door", "nameL1", 
						    "nameL2" )
				   ));
    $this->setCreateLinkTitle(_("Add a new counting sensor"));
    $bs = $this->getDialogTable();
    $bs->setTitle(_("Counting sensors configuration"));
    parent::setModuleName("BTopLocalServer");
  }    

  public function getTableInput($params)
  {
    $params = array_merge(array("clientId" => "",
				"channel" => 0,
				"counter" => 0,
				"host" => "",
				"descr" => "",
				"door" => null,
				"sens" => 1), $params);
    
    $rows = H();

    if(isset($params["id"]))
      {
	$rows->row( array( _("Id"),
			   H()->input()->withValue($params["id"])
			   ->withType("text")->withMaxLength(8)
			   ->setReadOnly(true)->withName("id"))
		    );
      }

    $rows->row( array( _("clientId"),
		       H()->input()->withValue($params["clientId"])
		       ->withType("text")->withMaxLength(8)
		       ->withName("clientId"))
		);


    $rows->row( array( _("Channel"),
		       H()->input()->withValue($params["channel" ])
		       ->withType("text")->withid("remoteChannel")
		       ->withName("channel"))
		);

    $rows->row( array( _("Counter"),
		       H()->input()->withValue($params["counter" ])
		       ->withType("text")->withName("counter" ))
		);

    $rows->row( array( _("Host"),
		       H()->input()->withValue($params["host" ])
		       ->withType("text")->withId("remoteHost")
		       ->withName("host"))
		);

    $rows->row( array( _("Descr"),
		       H()->input()->withValue($params["descr" ])
		       ->withType("text")->withName("descr"))
		);

    $module = new Doors($this->getModule());
    $doors = $module->getEntries();

    if(is_null($params["door"]))
      {
	$params["door"] = $doors[0]["id"];
      }

    $rows->row( array( _("Door"),
		       createSelect( $doors,
				     array( "name" => "door",
					    "key" => "id",
					    "value" => "name",
					    "default" => $params["door"],
					    "extra" => "onChange=\"BTopLocalServer.ModuleChangeDirection('BTopSensors','door')\"")))
		);

    $rows->row( array( _("Direction"),
		       createSelect( $module->getOrientedEntry(array( "id" => $params["door"] )),
				     array( "name" => "sens",
					    "key" => "sens",
					    "default" => $params["sens"],
					    "value" => "nameZ1,nameZ2",
					    "sep" => " => " )))
		); 

    $table = H()->table($rows);
    $table->input()->withType("hidden")->withName("type")->withValue("B-TOP");

    $gui = H()->form($table)->withId($this->getClassName() . "EditForm");

    $this->setDialogParams( array( "msg" => $gui->str() ));
    $this->setEditTitle(_("Edit counting sensor parameters"));
    $this->setCreateTitle(_("Add a new counting sensor"));
    parent::getTableInput($params);
  }

  public function delEntrySubmission()
  {
    $params = $this->getParams();
    $cs = $this->getModule()->getEntry( array( "id" => $params["id"] ) );
    $csName = $cs["ref"];
    $this->setDeleteTitle(_("Remove a counting sensor"));
    $this->setDialogParams(array( "msg" => H()->p(_("Are you sure to remove the counting sensor $csName ?"))->str()));
    parent::delEntrySubmission();
  }
}

?>