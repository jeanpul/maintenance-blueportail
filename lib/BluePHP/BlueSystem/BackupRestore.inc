<?php

include_once("BluePHP/Utils/Input.inc");

class BackupRestore
{
  public function __construct()
  {
  }

  public function getParamsList($module)
  {
    $res = array();
    $lines = array();
    exec("cat /etc/BEV/modules/$module/BackupRestore.cfg", $lines);
    foreach($lines as $l)
      {
	$elts = explode(" ", $l);
	if(is_array($elts) and count($elts) == 5)
	  {
	    $res[$elts[1]] = array( "module" => $elts[0],
				    "param" => $elts[1],
				    "title" => urldecode($elts[2]),
				    "backup_func" => $elts[3],
				    "restore_func" => $elts[4] );
	  }
      }
    return $res;
  }
  
  public function createBackup($module, $paramsList)
  {
    $query = "";
    if(file_exists("/etc/BEV/modules/$module/Functions.inc"))
      {
	$query .= ". /etc/BEV/modules/$module/Functions.inc\n";
      }
    else
      {
	$query .= ". /etc/BEV/scripts/BlueSystem/BackupRestore.inc\n";
      }
    $query .= "Backup_startPrepareExport\n";
    foreach($paramsList as $elt)
      {
	$query .= "BackupRestore_backupOfModuleLineWithoutSelectTest $module " . 
	  $elt["param"] . " " . urlencode($elt["title"]) . " " . $elt["backup_func"] . 
	  " " . $elt["restore_func"] . "\n";
      }
    $query .= "Backup_endPrepareExport\n";
    $query .= 'echo BACKUP_ArchiveName=$BACKUP_ArchiveName';
    exec($query, $output);
    $res = parseSimplePairValues($output);
    return $res["BACKUP_ArchiveName"];
  }

};

?>