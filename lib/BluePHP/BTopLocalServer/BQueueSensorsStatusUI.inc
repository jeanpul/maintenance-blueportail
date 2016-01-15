<?php

include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/BlueSystem/GUI/BSModuleInterface.inc");
include_once("BluePHP/BTopLocalServer/GrabIPTable.inc");

class BQueueSensorsStatusUI extends BSModuleInterface
{
  public function __construct($id = "", $params = array())
  {
    parent::__construct($id, $params);
    parent::setModule(new GrabIPTable());
    $this->setCreateLinks(false);
    $this->setDeleteLinks(true);
    $this->setEditLinks(false);
    $this->setRefreshButton(true);
    parent::setTableParams( array( "titles" => array( _("Location"), _("Ref"), _("Value"), _("Status") ),
				   "cols" => array( "location", "ref", "lastvalue", "laststatus" )
				   ));
    $bs = $this->getDialogTable();
    $bs->setTitle(_("B-Queue numbering sensors communication status"));
    parent::setModuleName("BTopLocalServer");
  }

  protected function getModuleEntries()
  {
    return $this->getModule()->checkEntriesForAllBQueues();
  }

  public function delEntrySubmission()
  {
    $params = $this->getParams();
    $cs = $this->getModule()->getEntry( array( "ref" => $params["ref"] ) );
    $csName = $cs["ref"];
    $this->setDeleteTitle(_("Remove a sensor status"));
    $this->setDialogParams(array( "msg" => H()->p(_("Are you sure to remove the status for the sensor $csName ?"))->str()));
    parent::delEntrySubmission();
  }
}

?>