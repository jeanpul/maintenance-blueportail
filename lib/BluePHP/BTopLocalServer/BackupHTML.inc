<?php

include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/BlueSystem/GUI/BSApplication.inc");
include_once("BluePHP/BlueSystem/BackupRestore.inc");
include_once("BluePHP/BlueSystem/GUI/BSDialogSubmit.inc");
include_once("BluePHP/BlueSystem/GUI/BSDialogInfo.inc");

class BackupHTML extends BSDialogSubmit
{
  public function __construct($id, $params)
  {
    parent::__construct($id, array(), $params);
    parent::setContent("BSDialogSubmit");
  }
  
  public function render()
  {
    $module = new BackupRestore();
    $paramsList = $module->getParamsList("BTopLocalServer");
    $liElts = H();
    foreach($paramsList as $v)
      {
	$liElts->li(array( H()->input()->withType("checkbox")->withName($v["param"])->withValue(1)->isChecked(true),
			   $v["title"] ));
      }
    $gui = H()->form( H()->ul( $liElts ) )->withId("BackupForm");
    $this->setTitle("Backup configuration");
    $this->setMsg($gui->str());
    $this->eventSubmit(BS_eventRenderWithForm("BTopLocalServer.BackupHTML", $this->getId(), "BackupForm", "prepareExport"));
    parent::render();
  }

  public function prepareExport()
  {
    $params = $this->getParams();
    // retrieve only checked parameters
    $module = new BackupRestore();
    $paramsList = $module->getParamsList("BTopLocalServer");
    $paramsListChecked = array();
    $liElts = H();
    foreach($paramsList as $k => $v)
      {
	if(isset($params[$k]) and $params[$k] == 1)
	  {
	    $paramsListChecked[$k] = $v;
	    $liElts->li( $v["title"] );
	  }
      }
    $gui = H();
    if(count($paramsListChecked))
      {
	$backupFile = $module->createBackup("BTopLocalServer", $paramsListChecked);
	$gui->h3(_("Backup content"))->ul($liElts)->h3(_("Obtain the archive"));
	$gui->p( H()->a($backupFile)->withHRef("/" . $backupFile)->withStyle("color: black") );
      }
    else
      {
	$gui->p( _("Nothing to backup") );
      }
    $dialog = new BSDialogInfo("BackupRestoreExport", array( "title" => "Backup export",
							     "msg" => $gui->str() ));
    $dialog->render();
  }

}

?>