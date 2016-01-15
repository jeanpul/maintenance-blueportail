<?php

include_once("BluePHP/EcoCompteur/EcoCompteurs.inc");

if(count($argv) < 3) 
  {
    die("Missing arguments : site (Perros|Concarneau|SaintQuayPortrieux), year (YYYY)\n");
  }

$site = $argv[1];
$year = $argv[2];

// cannot backup the current year !

$currentYear = strftime("%Y");

if((int) $year >= (int) $currentYear)
  {
    die("Cannot backup current or future years\n");
  }

echo "Backup site $site for year $year\n";

$ec = new EcoCompteurs();

$params = array( "ftp_server" => "ftp.phpnet.org",
		 "ftp_port" => 21,
		 "ftp_login" => "jeanpul_ecocompteur",
		 "ftp_passwd" => "ecocompteur",
		 "ftp_dir" => "$site" );

$con = ftp_connect($params["ftp_server"], $params["ftp_port"]);
if($con and 
   (!ftp_login($con, $params["ftp_login"], $params["ftp_passwd"]) or
    !ftp_chdir($con, $params["ftp_dir"])))
  {
    $con = false;
  }
else
  {
    $files = ftp_nlist($con, ".");

    if(in_array("backup_$year", $files))
      {
	echo "Directory backup_$year already exist\n";
      }
    else
      {
	echo "Create directory backup for $year\n";
	if(!ftp_mkdir($con, "backup_$year"))
	  {
	    echo "Error while creating directory for $year\n";
	  }
      }

    foreach($files as $f)
      {
	if(preg_match("/^$year/", $f) or preg_match("/^log_$year/", $f))
	  {
	    if(ftp_rename($con, $f, "backup_" . $year . "/" . $f))
	      {
		echo "file $f moved to backup_$year\n";
	      }
	  }
      }
  }

//$filesList = $ec->getFilesList( $params );

?>