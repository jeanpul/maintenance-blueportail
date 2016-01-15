<?php

include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/GUI/HTMLComponent.inc");
include_once("BluePHP/BlueSystem/GUI/BSModuleInterface.inc");
include_once("BluePHP/BTopLocalServer/GrabIPTable.inc");

class BTopSensorsStatusUI extends BSModuleInterface
{
  public function __construct($id = "", $params = array())
  {
    parent::__construct($id, $params);
    parent::setModule(new GrabIPTable());
    $this->setCreateLinks(false);
    $this->setDeleteLinks(true);
    $this->setEditLinks(false);
    $this->setRefreshButton(true);
    parent::setTableParams( array( "titles" => array( _("Door"), _("Ref"), _("Value"), _("Status") ),
				   "cols" => array( "door", "ref", "lastvalue", "laststatus" )
				   ));
    $bs = $this->getDialogTable();
    $bs->setTitle(_("B-Top counting sensors communication status"));
    parent::setModuleName("BTopLocalServer");
  }

  protected function getModuleEntries()
  {
    return $this->getModule()->checkEntriesForAllBTops();
  }
}

?>