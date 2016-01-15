<?php

include_once('BluePHP/BlueSystem/Tools/BSPropertyFile.inc');
include_once('ServerRef.inc');
include_once('Common.inc');

class ServerConfig
{
  public $basePath;
  public $serverRef;
  public $properties;
  
  /**
   * STATUS 
   * -1 : Undefined
   * 0 : unused
   * 1 : desactivated
   * 2 : activated
   */
  private $status = -1;
  
  
  
  public function __construct($basepath, $id = -1)
  {
    $this->basePath = $basepath;
    $clientId = Common_obtainHostClientId();
    $this->serverRef = new ServerRef($clientId, $id);
    $this->setStatusDependantElement();
  }
  
  private function setStatusDependantElement()
  {
    $this->status = $this->evalServerStatus();
    $this->properties = new BSPropertyFile($this->serverPath() . 'properties');
  }
  
  private function extractIdFromName($name)
  {
    return preg_replace('/.*server_/', '', $name);
  }
  
  public function serverPath()
  {
    $str = $this->basePath . '/';
    if($this->status == 1) $str .= 'desactivated';
    $str .= $this->serverRef->getServerName() . '/';
    return $str;
  }
  
  public function getProperty($property_name)
  {
    $this->properties->loadProperties();
    return $this->properties->getProperty($property_name);
  }
  
  public function getUserName()
  {
    return urldecode($this->getProperty('serverName'));
  }
  
  public function changeUserName($name)
  {
    $this->prepareWriteAccess();
    $this->properties->loadProperties();
    $this->properties->setProperty('serverName', urlencode($name));
    $this->properties->saveProperties();
  }
  
  public function addSensor($clientId, $id)
  {
    $this->prepareWriteAccess();
    $sensorRef = new SensorRef($this->serverRef, $clientId, $id);
    $sensordir = $this->serverPath() . $sensorRef->getSensorName();
    $sensordirconfig = $sensordir . "/config";
    if(!file_exists($sensordirconfig))
      {
	mkdir($sensordir);
	mkdir($sensordirconfig);
      }
  }
  
  public function removeSensor($clientId, $id)
  {    
    $this->prepareWriteAccess();
    $sensorRef = new SensorRef($this->serverRef, $clientId, $id);
    $sensordir = $this->serverPath() . $sensorRef->getSensorName();
    Common_recursiveDeleteFiles($sensordir);
  }
  
  public function getServerStatus()
  {
    return $this->status;
  }
  
  /**
   * -1 undefined
   * 0 ok
   * 1 pb with some channels
   * 2 pb with all channels
   */
  public function getServerProcessStatus()
  {
    $lastprocessdir = $this->getLastFinishedProcessing();
    if($lastprocessdir != "")
      {
	$data = file($lastprocessdir . "/valid.lst");
	$data = preg_grep('/^[a-zA-Z0-9]/', $data);
	$noklines = preg_grep('/^X / ', $data, PREG_GREP_INVERT);
	
	if(count($noklines) > 0)
	  {
	    return count($noklines) == count($data) ? 2 : 1;
	  }
	return 0;
      }
    return -1;
  }
  
  public function getLastFinishedProcessing()
  {
    $serverdir = $this->serverPath();
    $tmpdirlist = Common_tmpDirListFile($serverdir);
    $tmplist = Common_readTmpDirList($tmpdirlist);
    
    for($i = count($tmplist) - 1; $i >= 0; $i--)
      {
	if(!file_exists($tmplist[$i] . "/lock") &&
	   file_exists($tmplist[$i] . "/valid.lst"))
	  {
	    return $tmplist[$i];
	  }
      }
    
    return "";
  }
  
  private function initializeServer($activate = true)
  {
    //echo "initializeServer";    

    $serverpath = $this->basePath .'/';
    if(!$activate) $serverpath .= 'desactivated';
    $serverpath .= $this->serverRef->getServerName() . '/';

    mkdir($serverpath);
    $this->setStatusDependantElement();
    $this->properties->setAllProperties(array('serverName' => 'default+name'));
    $this->properties->saveProperties();
  }
  
  public function activate()
  {
    switch($this->status)
      {
      case 0:
	$this->initializeServer(true);
	break;
      case 1 :
	$activedir = $this->basePath .'/'. $this->serverRef->getServerName();
	$unactivedir = $this->basePath .'/'. 'desactivated' . $this->serverRef->getServerName() ;
	rename($unactivedir, $activedir); 
	$this->setStatusDependantElement();
	break;
      }
  }
  
  public function desactivate()
  {
    switch($this->status)
      {
	
      case 2 :
	$activedir = $this->basePath .'/'. $this->serverRef->getServerName();
	$unactivedir = $this->basePath .'/'. 'desactivated' . $this->serverRef->getServerName() ;
	rename($activedir, $unactivedir); 
	$this->setStatusDependantElement();
	break;
      }
  }
  
  private function evalServerStatus()
  {   
    
    if(file_exists($this->basePath .'/'. $this->serverRef->getServerName()))
      {
	return 2;
      }
    else if(file_exists($this->basePath .'/'. 'desactivated' . $this->serverRef->getServerName()))
      {
	return 1;
      }
    else return 0;
  }

  public function listServers()
  {
    $clientId = $this->serverRef->clientId;
    $serverset = array();
    $handle = opendir($this->basePath);
    while($str = readdir($handle))
      {
	if(preg_match('/^server_/', $str))
	{
	  $s = new ServerConfig($this->basePath, $clientId, $this->extractIdFromName($str));
	  $s->activated = true;
	  array_push($serverset, $s);
	}
      }
    closedir($handle);
    return $serverset;
  }
  
  public function listSensors()
  {
    $sensorlist = array();
    if(is_dir($this->serverPath()))
      {
        foreach(glob($this->serverPath() . "/sensor_*", GLOB_ONLYDIR) as $sensorPath)
	  {
	    $sensor = new SensorRef($this->serverRef);
	    $sensor->setFromName($sensorPath);
	    array_push($sensorlist, $sensor);
	  }
      }
    return $sensorlist;
  }

  /** check if it is a unused server
   * then create the directory as desactivated server to enable
   * to save the configuration done before activation
   */
  private function prepareWriteAccess()
  {
    if($this->status == 0)
      {
	$this->initializeServer(false);
      }
  }
}

?>