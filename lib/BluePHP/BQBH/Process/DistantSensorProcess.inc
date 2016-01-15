<?php

include_once('ClientIdConfig.inc');

class DistantSensorProcess
{
  public $basePath;
  public $sensorRef;
 
  /* current processing key used to identify computationy*/
  public $currentProcessingKey;
  public $state;
  
  private $lastActionRessource = 0;
  private $lastActionEndStatus = 0;

  /**
   * paramMethod : method to cal to complete the command line of default parameters
   * distantMethod : method to call on sensor host
   */
  private $actionsMethod = array(
				 "updateConfig" => array("paramMethod" => "updateConfigParam"),
				 "grabImage" => array("paramMethod" => ""),
				 "imageProcessing" => array("paramMethod" => ""));
  
  public function __construct($basePath, $serverRef, $sensor_name)
  {
    $this->basePath = $basePath;
    $this->sensorRef = new SensorRef($serverRef);
    $this->sensorRef->setFromName($sensor_name);
    
  }
  
  /**
   * serverClientId
   * serverId
   * timestamp
   * action
   */
  public function askAction($action)
  { 
    $this->lastActionRessource = -1;
    $this->lastActionEndStatus = -1;
    
    $request_params = array("serverClientId" => $this->sensorRef->serverRef->clientId,
			    "serverId" => $this->sensorRef->serverRef->id,
			    "action" => $action,
			    "sensorId" => $this->sensorRef->id,
			    "sensorClientId" => $this->sensorRef->clientId,
			    "processingKey" => $this->currentProcessingKey);
        
    if($this->actionsMethod[$action]["paramMethod"] != "")
      {
	$method = $this->actionsMethod[$action]["paramMethod"];
	$complement_params = $this->$method();
	$request_params = array_merge($request_params, $complement_params);
      }
   
    
    $urlparam = "";
    foreach($request_params as $n => $v)
      {
	if($urlparam != "") $urlparam .= "&";
	$urlparam .= $n . "=" . $v;
      }
    
    $str = BQBHSERVER_DISTANT_SENSOR_ACTION_BIN . "?" . $urlparam;
    echo "->$str\n";

    $this->lastActionRessource = popen(BQBHSERVER_ACTION_EXEC_BIN . " " . $this->basePath . " " . $this->sensorRef->clientId . " \"$str\"", "r");
  }

  public function waitActionAnswer($output_file)
  {
    $handle = fopen($output_file, "a");
    while(!feof($this->lastActionRessource))
      {
	$line = fgets($this->lastActionRessource);
	fwrite($handle, $line);
      }
    $this->lastActionEndStatus = pclose($this->lastActionRessource);
    fclose($handle);
    
    return $this->lastActionEndStatus;
  }

  public function retrieveProcessingFile($params)
  {
    $this->lastActionRessource = -1;
    $this->lastActionEndStatus = -1;

    $url = Common_tmpDirOnSensorSide(BQBHSENSOR_WWW_PATH,  
				     $this->sensorRef,
				     $this->currentProcessingKey) .
	$params["imgname"];

/*
    echo "-$url-\n";
    $dest = fopen($params["outputfile"], "wb");
    $clienthostparamfile = $this->hostparamfile;
    $this->lastActionRessource = popen(BQBH_ACTION_EXEC_BIN . " $clienthostparamfile \"$url\"", "r");
    while(!feof($this->lastActionRessource))
      {
	fwrite($dest, fread($this->lastActionRessource, 1024));
      }
    $this->lastActionEndStatus = pclose($this->lastActionRessource);
    fclose($dest);
  */
    $client = new ClientIdConfig($this->basePath, $this->sensorRef->clientId);
    $url = "http://" . $client->getHostIP() . $url;
    $ret = Common_downloadFile($url, $params['outputfile'], BQBHSERVER_SENSOR_REQUEST_TIMEOUT);
    $this->lastActionEndStatus = $ret;
  }

  public function getLastActionStatus()
  {
    return $this->lastActionEndStatus;
  }


  private function updateConfigParam()
  {  
    $sensordir = Common_sensorDirOnServerSide($this->basePath, $this->sensorRef);
    $configarchive = $sensordir . "/config.tgz";
  
    if(!file_exists($configarchive))
      {
	exec(BQBHSERVER_CREATE_CONFIG_ARCHIVE_BIN . " $sensordir");
      }
    $md5 = md5_file($configarchive);
    return array("md5" => $md5);
  }

}

?>