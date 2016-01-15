<?php

include_once("BluePHP/GUI/Window.inc");
include_once("EcoCompteurs.inc");
include_once("Status.inc");

class StatusHTML extends Window
{
  public function __construct($params)
  {
    parent::__construct("Status", "Status");
  }

  // display the files found for each
  // EcoCompteur
  function getFTPFiles()
  {
    $str = "";
    $module = new EcoCompteurs();
    $statusModule = new Status();
    foreach($module->getEntries() as $k => $v)
      {
	$str .= "<h2>Counter " . $v["id"] . " / " . $v["clientId"] . "</h2>\n";
	$str .= "<table border=\"1\">\n";
	$str .= "<tr><th>File</th><th>Date</th></tr>\n";
	foreach($module->getFilesList($v) as $f => $g)
	  {
	    // file name must be in the form
	    // YYYYMMDD.csv
	    if($module->isCountingFileName($g))
	      {		
		$status = $statusModule->getEntry( array( "counters_id" => $v["id"],
							  "file" => substr($g, 0, 8) ));		
		$ftpfile = "ftp://" . $v["ftp_login"] . ":" . $v["ftp_passwd"] . 
		  "@" . $v["ftp_server"] . ":" . $v["ftp_port"] . "/" . $v["ftp_dir"] . "/$g";
		$str .= "<tr><td><a href='$ftpfile'>" . $g . "</a></td><td>" . ($status ? $status["lastDate"] : "NP") . "</td></tr>\n";		
	      }
	  }
	$str .= "</table>\n";
      }
    return $str;
  }
}

?>
