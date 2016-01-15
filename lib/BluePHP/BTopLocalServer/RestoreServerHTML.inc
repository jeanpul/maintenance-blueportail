<?php

include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/BlueSystem/GUI/BSApplication.inc");
include_once("BluePHP/BTopLocalServer/ServerSync.inc");
include_once("BluePHP/BlueSystem/GUI/BSDialogSubmit.inc");
include_once("BluePHP/BlueSystem/GUI/BSDialogInfo.inc");

class RestoreServerHTML extends BSDialogSubmit
{
  public function __construct($id, $params)
  {
    parent::__construct($id, $params);
    parent::setContent("BSDialogSubmit");
  }

  public function render()
  {    
    $gui = H()->h3(_("Import data from an existing QMS server"))->form( array( _("Server adress"), 
									       H()->input()->withType("text")->withName("SYNC_SERVER_RESTORE") )
									)->withId("RestoreForm");
    $this->setTitle(_("Upload a backup from an existing QMS server"));
    $this->setMsg($gui->str());
    $this->eventSubmit(BS_eventRenderWithForm("BTopLocalServer.RestoreServerHTML", $this->getId(), "RestoreForm", "applyImport"));
    parent::render();
  }

  public function applyImport()
  {
    $params = $this->getParams();
    if($params["SYNC_SERVER_RESTORE"] != "")
      {
	$gui = H()->p(_("Start retrieving data from the server"));
	$gui->form(H()->input()->withType("hidden")->withName("SYNC_SERVER_RESTORE")->withValue($params["SYNC_SERVER_RESTORE"] ))->withId("RestoreForm");
	$dialog = new BSDialogInfo($this->getId(), array( "title" => _("Backup restore sync"),
								  "msg" => $gui->str() ));
	$dialog->afterCreate(BS_eventRenderWithForm("BTopLocalServer.RestoreServerHTML", $this->getId(), "RestoreForm", "processImport"));
	$dialog->render();
      }
    else
      {
	$dialog = new BSDialogInfo("RestoreError", array("title" => _("Backup restore error"), 
							 "msg" => H()->p(_("Error: empty server adress."))));
	$dialog->render();
      }
  }

  public function processImport()
  {
    $params = $this->getParams();
    $module = new ServerSync();
    $res = $module->restore($params["SYNC_SERVER_RESTORE"]);
    $dialogProcess = new BSDialogInfo($this->getId());
    $dialog = null;
    if($res == 0)
      {
	$dialog = new BSDialogInfo("RestoreInfo", array("title" => _("Backup restore done"), 
							"msg" => H()->p(_("Restoration finished."))));
      }
    else
      {
	$dialog = new BSDialogInfo("RestoreError", array("title" => _("Backup restore error"), 
							 "msg" => H()->p(_("Restoration Failed."))));
      }
    $dialog->beforeCreate($dialogProcess->eventClose());
    $dialog->render();
  }

}

?>