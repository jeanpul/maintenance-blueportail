<?php

include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/BlueSystem/GUI/BSApplication.inc");
include_once("BluePHP/BlueSystem/BackupRestore.inc");
include_once("BluePHP/BlueSystem/GUI/BSDialogSubmit.inc");
include_once("BluePHP/BlueSystem/GUI/BSDialogInfo.inc");

class RestoreHTML extends BSDialogSubmit
{
  public function __construct($id, $params)
  {
    parent::__construct($id, $params);
    parent::setContent("BSDialogSubmit");
  }

  public function render()
  {    
    $gui = H()->h3(_("Upload a backup file"))->form( H()->input()->withType("file")->withName("upfile")
						     )->withId("RestoreForm")->withMethod("POST")->withEncType("multipart/form-data");
    $this->setTitle(_("Upload a backup file"));
    $this->setMsg($gui->str());
    $this->eventSubmit(BS_eventRenderWithForm("BTopLocalServer.RestoreHTML", $this->getId(), "RestoreForm", "prepareImport"));
    parent::render();
  }

  public function prepareImport()
  {
    if(isset($_FILES["upfile"]))
      {
	$RESTORE_TMPFILE="/tmp/tmpRestore.tgz";
	if(move_uploaded_file($_FILES["upfile"]["tmp_name"], $RESTORE_TMPFILE))
	  {
	    $restoreMD5 = exec('$(md5sum ' . $RESTORE_TMPFILE . ' | awk -F " " \'{printf $1}\'' . ")");
	    $str = "source /etc/BEV/scripts/BlueSystem/BackupRestore.inc\n" . 
	      "Restore_extractDataFromImport " . $RESTORE_TMPFILE . "\n" .
	      'echo RESTORE_RES=$?' . "\n" . 
	      'echo RESTORE_ArchiveDate=$RESTORE_ArchiveDate' . "\n" . 
	      'echo RESTORE_ArchiveRef=$RESTORE_ArchiveRef' . "\n";
	    $output = array();
	    exec($str, $output);
	    $res = parseSimplePairValues($output);
	    $errorMsg = "";
	    if($res["RESTORE_RES"] == 1)
	      {
		$errorMsg = _("Unable to extract archive");
	      }
	    else if($res["RESTORE_RES"] == 2)
	      {
		$errorMsg = _("Incorrect archive : unable to find BackupSettings/Content.txt file");
	      }
	    else
	      {
		echo "<p>Date : " . $res["RESTORE_ArchiveDate"] . "</p>\n";
		echo "<p>Reference : " . $res["RESTORE_ArchiveRef"] . "</p>\n";
	      }
	    
	    if($res["RESTORE_RES"] == 0)
	      {		
		system("(cd /tmp && tar zxf $RESTORE_TMPFILE BackupSettings/BackupRestore.cfg)");
		$CONTENTTMPFILE="/tmp/BackupSettings/BackupRestore.cfg";
		$str = "source /etc/BEV/scripts/BlueSystem/BackupRestore.inc\n" . 
		  'echo $(Restore_processModuleList ' . $CONTENTTMPFILE . ' Restore_HTML_inputModuleLine)' . "\n";
		$gui = H()->p(_("Date : ") . $res["RESTORE_ArchiveDate"])->p(_("Reference : ") . $res["RESTORE_ArchiveRef"]);
		$gui->h3(_("Select elements to import from the backup"));
		$gui->form(exec($str))->withId("RestoreForm");
		$this->setTitle(_("Upload a backup file"));
		$this->setMsg($gui->str());
		$this->eventSubmit(BS_eventRenderWithForm("BTopLocalServer.RestoreHTML", $this->getId(), "RestoreForm", "applyImport"));
		parent::render();
              }
	    else
	      {
		$dialog = new BSDialogInfo("RestoreError", array("title" => _("Backup restore error"), 
								 "msg" => H()->h3($errorMsg)->p( "<p>Your archive seems an old backup and " . 
												 "is not supported by this system version. "
												 . "Please contact BlueEyeVideo")->str() ));	    
		$dialog->render();
	      }
	  }
	else
	  {
	    $dialog = new BSDialogInfo("RestoreError", array("title" => _("Backup restore error"), 
							     "msg" => _("Cannot create temporary restore file")) );
	    $dialog->render();
	  }
      }
    else
      {
	$dialog = new BSDialogInfo("RestoreError", array("title" => _("Backup restore error"), 
							 "msg" => _("No backup file found.")) );
	$dialog->render();
      }

  }

  public function applyImport()
  {
    $str = "source /etc/BEV/scripts/BlueSystem/BackupRestore.inc\n" . 
      "Restore_startImport\n" . 
      'echo RESTORE_RES=$?' . "\n" . 
      'echo RESTORE_Dir=$RESTORE_Dir' . "\n";
    $output = array();
    exec($str, $output);
    $res = parseSimplePairValues($output);

    if($res["RESTORE_RES"] != "0")
      {
	$dialog = new BSDialogInfo("RestoreError", array("title" => _("Backup restore error"), 
							 "msg" => _("Unable to find BackupSettings.")) );
	$dialog->render();
      }
    else
      {
	$strParams = "";
	foreach($params as $k => $v)
	  {
	    $strParams .= "QS_" . $k . "=" . $v . "\n";
	  }
	$str = "source /etc/BEV/scripts/BlueSystem/BackupRestore.inc\n" . 
	  $strParams . 
	  'echo $(BackupRestore_processModuleList BlueSystem Restore_HTML_selectedModuleLine)' . "\n";
	echo exec($str);

	$str = "source /etc/BEV/scripts/BlueSystem/BackupRestore.inc\n" . 
	  $strParams . 
	  'BackupRestore_loadNeededFunctionForModule BlueSystem' . "\n" .  
	  'Restore_processModuleList ' . $params["RESTORE_Dir"] . '/BackupRestore.cfg Restore_restoreOfModuleLine' . "\n" .
	  'Restore_endImport' . "\n";
	exec($str);
	$dialog = new BSDialogInfo("RestoreError", array("title" => _("Backup restore done"), 
							 "msg" => _("Backup import from file done.")) );
	$dialog->render();
      }
  }
}

?>