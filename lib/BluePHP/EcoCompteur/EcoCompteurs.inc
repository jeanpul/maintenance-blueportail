<?php

include_once("BluePHP/Utils/ModuleProcessing.inc");

class EcoCompteurs extends ModuleProcessing
{
  private $colNameString = "E1,S1,E2,S2,E3,S3,E4,S4,E5,S5,E6,S6,E7,S7,E8,S8";

  function EcoCompteurs()
  {
    parent::setParams( array( "name" => "counters",
			      "keyname" => "id",
			      "sort" => "clientId" ));

  }

  function getFilesList($params)
  {
    $res = array();
    $con = $this->getFTPConnection($params);
    if($con)
      {
	$res = ftp_nlist($con, ".");
      }
    return $res;
  }

  function getFileContent($params, $date)
  {
    $str = array();
    $con = $this->getFTPConnection($params);
    if($con)
      {
	$filename = $this->getCountingFileName($date);
	$temp = tmpfile();
	if(ftp_fget($con, $temp, $filename, FTP_BINARY))
	  {	    	 
	    fseek($temp, 0);
	    $numColValue0 = false;
	    $numColValue1 = false;
	    $numColDate = 0;
	    $numColHour = 1;
	    $haveNumCols = false;
	    while(!feof($temp))
	      {
		$line = stream_get_line($temp, 1024, "\n");
		if($haveNumCols)
		  {
		    $colValues = explode(",", $line);		    
		    // convert date into YYY-MM-DD HH:MM:SS
		    $str[] = array( "date" => substr($colValues[$numColDate], 6, 4) . "-" . 
				    substr($colValues[$numColDate], 3, 2) . "-" . 
				    substr($colValues[$numColDate], 0, 2) . " " .
				    $colValues[$numColHour],
				    "value0" => $colValues[$numColValue0],
				    "value1" => $colValues[$numColValue1] );
		  }
		else if(stripos($line, $this->colNameString))
		  {
		    $colNames = explode(",", $line);
		    $numColValue0 = array_search($params["colCounter0"], $colNames);
		    $numColValue1 = array_search($params["colCounter1"], $colNames);
		    $numColDate = 0;
		    $numColHour = 1;
		    $haveNumCols = true;
		  }
	      }
	  }
	fclose($temp);
      }
    return $str;
  }

  function isCountingFileName($filename)
  {
    $data = explode(".", $filename);
    return ($data and 
	    count($data) == 2 and 
	    strlen($data[0]) == 8 and 
	    $data[1] == "csv" and
	    mktime(0, 0, 0,
		   substr($data[0], 4, 2),
		   substr($data[0], 6, 2),
		   substr($data[0],0, 4)));
  }
  
  function getCountingFileName($date)
  {
    return $date . ".csv";
  }

  private function getFTPConnection($params)
  {
    $con = ftp_connect($params["ftp_server"], $params["ftp_port"]);
    if($con and 
       (!ftp_login($con, $params["ftp_login"], $params["ftp_passwd"]) or
	!ftp_chdir($con, $params["ftp_dir"])))
      {
	$con = false;
      }
    return $con;
  }


}

?>
