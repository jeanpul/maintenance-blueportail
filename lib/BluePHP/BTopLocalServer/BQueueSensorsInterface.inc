<?php

include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/BlueSystem/GUI/BSModuleInterface.inc");
include_once("BluePHP/BTopLocalServer/BQueueSensors.inc");

class BQueueSensorsInterface extends BSModuleInterface
{
  public function __construct($id = "", $params = array())
  {
    parent::__construct($id, $params);
    parent::setModule(new BQueueSensors());
    parent::setTableParams( array( "titles" => array( _("Id"), _("Ref"), _("Host"), _("Descr"), _("Location") ),
				   "cols" => array( "id", "ref", "host", "descr", "location" )
				   ));
    $this->setCreateLinkTitle(_("Add a new numbering sensor"));

    $bs = $this->getDialogTable();
    $bs->setTitle(_("Numbering sensors configuration"));

    parent::setModuleName("BTopLocalServer");
  }

  public function getTableInput($params)
  {
    $params = array_merge(array("clientId" => "",
				"channel" => 0,
				"region" => 0,
				"host" => "",
				"descr" => "" ), $params);

    $rows = H();

    if(isset($params["id"]))
      {
	$rows->row( array( _("Id"),
			   H()->input()->withValue($params["id"])
			   ->withType("text")->withMaxLength(8)
			   ->setReadOnly(true)->withName("id"))
		    );
      }

    $rows->row( array( _("ClientId"),
		       H()->input()->withValue($params["clientId"])
		       ->withType("text")->withName("clientId"))
		);

    $rows->row( array( _("Channel"),
		       H()->input()->withValue($params["channel" ])
		       ->withType("text")->withId("remoteChannel")
		       ->withName("channel"))
		);

    $rows->row( array( _("Region"),
		       H()->input()->withValue($params["region" ])
		       ->withType("text")->withName("region"))
		);

    $rows->row( array( _("Host"),
		       H()->input()->withValue($params["host" ])
		       ->withType("text")->withId("remoteHost")
		       ->withName("host"))
		);

    // TODO : should implements a tr( H()->td() ) in HTML
    //    $rows->row( array( "<tr><td colspan=\"2\"><input type=\"button\" value=\"View remote sensor configuration\" " .
    //		       "onClick=\"BTopLocalServer_viewBTopSensorConfig(document.getElementById('remoteChannel').value, " .
    //		       "document.getElementById('remoteHost').value)\"></td></tr>\n" ));
    
    $rows->row( array( _("Descr"),
		       H()->input()->withValue($params["descr" ])
		       ->withType("text")->withName("descr"))
		);

    $rows->row( array( _("Location"),
		       createSelect( $this->getModule()->getLocations(),
				     array( "key" => "id",
					    "name" => "location",
					    "value" => "name",
					    "default" => isset($params["location"]) ? $params["location"] : null,
					    "extra" => false ))
		       )
		);

    $table = H()->table($rows);
    $table->input()->withType("hidden")->withName("type")->withValue("B-QUEUE");

    $gui = H()->form($table)->withId($this->getClassName() . "EditForm");

    $this->setDialogParams( array( "msg" => $gui->str() ));
    $this->setEditTitle(_("Edit numbering sensor parameters"));
    $this->setCreateTitle(_("Add a new numbering sensor"));
    parent::getTableInput($params);
  }

  public function delEntrySubmission()
  {

    $params = $this->getParams();
    $cs = $this->getModule()->getEntry( array( "id" => $params["id"] ) );
    $csName = $cs["ref"];
    $this->setDeleteTitle(_("Remove a numbering sensor"));
    $this->setDialogParams(array( "msg" => H()->p(_("Are you sure to remove the numbering sensor $csName ?"))->str()));
    parent::delEntrySubmission();
  }
}



?>
