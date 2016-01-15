<?php

include_once("BluePHP/BlueSystem/BlueSystem.inc");
include_once("BluePHP/BlueSystem/Tools/BSLicenseFile.inc");

class BQBHServerLicense
{
  private $params = array( "FILE_SERVER" => "/etc/BEV/modules/BQBH/license_bqbhserver.txt",
			   "FILE_SENSOR" => "/etc/BEV/modules/BQBH/license_bqbh.txt" );

  public function haveLicenseFiles()
  {
    return file_exists($this->params["FILE_SERVER"]) and
      file_exists($this->params["FILE_SENSOR"]);
  }

  public function nbServerMax()
  {
    return $this->getNbFromLicense($this->params["FILE_SERVER"]);
  }

  public function nbSensorMax()
  {
    return $this->getNbFromLicense($this->params["FILE_SENSOR"]);
  }

  protected function getNbFromLicense($filename)
  {
    $nbMax = 0;
    if(file_exists($filename))
      {
	$bsl = new BSLicenseFile($filename);
	$bs = new BlueSystem();
	if($bsl->load($bs->obtainMACAddress()) == 0)
	  {
	    $nbMax = $bsl->getNb();
	  }
	else throw new Exception("Error while reading license file $filename.");
    }
    return $nbMax;    
  }
}

?>