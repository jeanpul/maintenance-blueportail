<?php

include_once('Common.inc');
include_once('SensorRef.inc');

class ClientIdConfig
{
  public $basePath;
  public $clientId;

  private $properties;
  
  public function __construct($base_path, $client_id = "")
  {
    $this->basePath = $base_path;
    $this->clientId = $client_id;

    $this->properties = new BSPropertyFile($this->getHostParamFile());
  }
  
  public function getName()
  {
    return "client_" . $this->clientId;
  }
  
  public function getClientIdDir()
  {
    return $this->basePath . "/" . $this->getName() . "/";
  }
  
  public function getHostParamFile()
  {
    return $this->getClientIdDir() . "host";
  }
  
  public function getHostIP()
  {
      $this->properties->loadProperties();
      return $this->properties->getProperty('CLIENTHOSTBQBH_IP');
  }
  
  public function isDefined()
  {
    return file_exists($this->getHostParamFile());
  }
   
  public function create($params)
  {
    if(!file_exists($this->getClientIdDir()))
      {
	mkdir($this->getClientIdDir());
      }
    
    $this->properties->setAllProperties($params);
    $this->properties->saveProperties();
  }
  
    public function updateAndSaveProperties($params)
	{
	    $this->properties->loadProperties();
	    $this->properties->updateProperties($params);
	    $this->properties->saveProperties();
	}

  public function remove()
  {
    if(file_exists($this->getClientIdDir()))
      {
	Common_recursiveDeleteFiles($this->getClientIdDir());
      }
  }
  
  public function checkSensorDependanciesInServerDir($serverdir)
  {
    $sensor = new SensorRef(false);
    $nb = 0;
    $handle = opendir($serverdir);
    if($handle)
      {
	while($str = readdir($handle))
	  {
	    if(preg_match('/^sensor_/', $str))
	    {
	      $sensor->setFromName($str);
	      if($sensor->clientId == $this->clientId) 
		{
		  $nb++;
		}
	    }
	  }
	closedir($handle);
      }
    return $nb;
  }
  
  public function checkSensorDependancies()
  {
    $nb = 0;
    $handle = opendir($this->basePath);
    if($handle)
      {
	while($str = readdir($handle))
	  {
	    if(preg_match('/^server_/', $str) || 
	       preg_match('/^desactivatedserver_/', $str))
	      {
		$nb += $this->checkSensorDependanciesInServerDir($this->basePath . "/" . $str);
	      }
	  }      
	closedir($handle);
      }
    return $nb;
  }


  
  public function nameToId($name)
  {
    return preg_replace('/client_/', '', $name);
  }
  
  public function listDefinedClients()
  {
    $clients = array();
    $handle = opendir($this->basePath);
    while($str = readdir($handle))
      {
	if(preg_match('/^client_.*/', $str))
	  {
	    array_push($clients, new ClientIdConfig($this->basePath, $this->nameToId($str)));
	  }
      }
    return $clients;
  }
  
}

?>
