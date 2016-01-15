<?php

include_once("BluePHP/BlueSystem/Tools/BSPropertyFile.inc");
include_once('BluePHP/BlueSystem/Tools/BSLicenseFile.inc');
include_once("BluePHP/System/Network.inc");

class BlueSystem
{
  private $params = array( "REFERENCE" => null,
			   "PROPERTIES_FILE" => "/etc/BEV/modules/BlueSystem/blueSystem.rc", 
			   'LANG_FILE' => "/etc/BEV/Lang.rc",
			   'COPYRIGHT_YEAR'=> "2012",
			   'RUNNING' => null,
			   'MAC' => null );

  private $lfile = null;

  public function getReference()
  {
    if(is_null($this->params["REFERENCE"]))
      {
	$references = array();
	// Actually reference value is based on the mac address
	$mac = $this->obtainMACAddress();
	if($mac == "")
	  {
	    $mac = "none";
	  }
	$references["MAC"] = $mac;
	$pFile = new BSPropertyFile($this->params["PROPERTIES_FILE"]);
	$pFile->loadProperties();
	$references["BLUESYSTEM_APPLI"] = $pFile->getProperty('BLUESYSTEM_APPLI');
	$references["BLUESYSTEM_VERSION"] = $pFile->getProperty('BLUESYSTEM_VERSION');
	$references["BLUESYSTEM_REVISION"] = $pFile->getProperty('BLUESYSTEM_REVISION');
	$this->params["REFERENCE"] = $references;
      }
    return $this->params["REFERENCE"];
  }

  public function getClientId()
  {
    $pFile = new BSPropertyFile($this->params["PROPERTIES_FILE"]);
    $pFile->loadProperties();
    $clientId = $pFile->getProperty('BLUESYSTEM_CLIENTID');
    if($clientId == "")
      {
	$mac = $this->obtainMACAddress();
	if($mac == "")
	  {
	    $mac = "none";
	  }
	$clientId = substr(md5($mac) . "\n",0,8);
	$this->setClientId($clientId);
      }
    return $clientId;
  }

  public function setClientId($id)
  {
    $pFile = new BSPropertyFile($this->params["PROPERTIES_FILE"]);
    $pFile->loadProperties();
    $pFile->setProperty('BLUESYSTEM_CLIENTID', $id);    
    $pFile->saveProperties();
  }

  public function getCopyrightYear()
  {
    return $this->params["COPYRIGHT_YEAR"];
  }

  /**
   * return whether or not
   * the system run on a ramdisk
   */
  public function isLive()
  {
    $this->params["RUNNING"] = is_dir("/KBEV") ? "cd": "hd";
    return $this->params["RUNNING"] == "cd";
  }

  public function obtainMACAddress($interface = "eth0")
  {
    $module = new Network();
    return $module->obtainMACAddress($interface);
  }

  public function loadLicenseFile($filename)
  {
    $this->lfile = new BSLicenseFile($filename);
    $ret = $this->lfile->load($this->obtainMACAddress());
    return $ret;
  }

  public function getLicenseFile()
  {
    return $this->lfile;
  }

  public function checkStatus()
  {
    $lines = array();
    $res = array();
    exec("source /etc/BEV/scripts/BlueSystem/BlueSystem.inc && BlueSystem_checkStatus 2> /dev/null", $lines);
    foreach($lines as $l)
      {
	list($id, $status, $msg) = explode(" ", $l);
	$res[] = array( "id" => $id, "status" => $this->getSymbolFromStatus($status), "msg" => urldecode($msg) );
      }
    return $res;
  }

  private function getSymbolFromStatus($status)
  {
    $symbols = array( "1" => "running",
		      "2" => "runningfalse",
		      "3" => "stopped",
		      "4" => "errorstatus" );
    return $symbols[$status];
  }
}

?>