<?php

include_once("ServerRef.inc");
include_once("SensorRef.inc");
include_once("Common.inc");
include_once("DistantSensorProcess.inc");

class ServerProcess
{
  public $serverRef;
  public $basePath;
  public $baseWWWPath;
  public $currentProcessingKey;
  
  // contains sensor obj to communicate
  // action to the sensor
  public $sensorList = array();
  
  // sensor key of the sensor still valid after computing
  public $validSensorList = array();
  
  private $startTime;
  private $endTime;

  private $maxLockTime = 600;
  private $lockPath = "";

  public function __construct($clientId, $id)
  {
    $this->serverRef = new ServerRef($clientId, $id);
    $this->basePath = BQBHSERVER_PATH;
    $this->baseWWWPath = BQBHSERVER_WWW_PATH;
    $this->lockPath = "/tmp/BQBH_SP_" . $this->serverRef->getServerName() . ".lock";
  }
 
  public function trylock()
  {
    $haveLock = false;
    $time = explode(" ", microtime());
    $time = $time[1] + $time[0];
    if(file_exists($this->lockPath))
      {
	$timePrevLock = file_get_contents($this->lockPath);
	if($time - $timePrevLock > $this->maxLockTime)
	  {
	    $haveLock = true;
	  }
      }
    else
      {
	$haveLock = true;
      }
    if($haveLock)
      {
	$handle = fopen($this->lockPath, "w");
	fwrite($handle, $time);
      } 
    return $haveLock;
  }

  public function unlock()
  {
    if(file_exists($this->lockPath))
      {
        unlink($this->lockPath);
      }
  }
 
  /** Return the path to the server directory */
  public function serverPath()
  {
    return Common_serverDirOnServerSide($this->basePath, $this->serverRef); 
  }
  
  public function serverWWWPath()
  {
    return Common_serverDirOnServerSide($this->baseWWWPath, $this->serverRef);
  }
  
  public function sensorPath($sensorRef)
  {
    return Common_sensorDirOnServerSide($this->basePath, $sensorRef);
  }
  
  /** Return the path to the tmp directory */
  private function tmpDir()
  {
    return Common_tmpDir($this->serverPath(), $this->currentProcessingKey);
  }
  
  /** Return the path to the tmp directory */
  private function tmpURL()
  {
    return Common_tmpDir($this->serverWWWPath(), $this->currentProcessingKey);
  }
  
  private function projImageName($sensorRef)
  {
    $id = $sensorRef->id;
    $clientId = $sensorRef->clientId;
    return $this->tmpDir() . "proj_" . $clientId . "_" . $id . ".ppm";
  }
  
  private function imageName($sensorRef)
  {
    $id = $sensorRef->id;
    $clientId = $sensorRef->clientId;
    return $this->tmpDir() . "img_" . $clientId . "_" . $id . ".jpg";
  }
  
  /** Return the lock file name */
  private function lockFileStr()
  {
    return $this->tmpDir() . "lock";    
  }
  
  public function actionResultFilename($sensorRef, $action_key)
  {
    return $this->tmpDir() . $action_key . "_" . $sensorRef->getSensorName() . ".txt";
  }
  
  /** Create the lock file to signal the processing */
  private function processLock()
  {
    $lockfile = $this->lockFileStr($this->currentProcessingKey);
    if($handle = fopen($lockfile,  "w"))
      {
	$date = Common_timestamp();
	fwrite($handle, $date);
	fclose($handle);
      }
    else 
      {
	throw new Exception("Cannot lock process file : " . $lockfile);
      }
  }
  
  /** Remove the lock file to signal the processing */
  private function processUnlock()
  {
    $lockfile = $this->lockFileStr();
    unlink($lockfile);
  }
  
  /** List the available sensors*/
  private function createSensorList()
  {
    $handle = opendir($this->serverPath());
    while($str = readdir($handle))
      {
	if(preg_match('/^sensor_.*/', $str))
	  {
	    $sensor = new DistantSensorProcess($this->basePath, $this->serverRef, $str);   
	    $this->sensorList[$str] = $sensor;
	  }
      }
    closedir($handle);
  }
  
  public function askActionToAllValidSensors($action)
  {   
    foreach($this->validSensorList as $s)
      {	
	$this->sensorList[$s]->askAction($action);
      }
  }
  
  public function waitAnswerFromAllValidSensors($action)
  {
    foreach($this->validSensorList as $s)
      {
	$output_file = $this->actionResultFilename($this->sensorList[$s]->sensorRef, $action);
	$this->sensorList[$s]->waitActionAnswer($output_file);
      }
  }
  
  
  public function updateValidSensorList()
  {
    $this->validSensorList = array();
    foreach($this->sensorList as $s => $r)
      {
	if($r->state == "X")
	  {
	    array_push($this->validSensorList, $s);
	  }
      }
  }
  
  public function saveSensorState()
  {
    $file = $this->tmpDir() . "/valid.lst";
    $handle = fopen($file, "w");
    foreach($this->sensorList as $s => $r)
      {
	fwrite($handle, $r->state . " " . 
	       $r->sensorRef->clientId . " " . 
	       $r->sensorRef->id . "\n");
      }
    fclose($handle);
  }
  
  public function doActionOnDistantSensors($action)
  {
    echo "Step : $action \n";
    $this->updateValidSensorList();
    $this->askActionToAllValidSensors($action);    
    
    $this->waitAnswerFromAllValidSensors($action);    
    
    //update sensor state according action result
    foreach($this->validSensorList as $s)
      {
	if($this->sensorList[$s]->getLastActionStatus() != 0)
	  {
	    $this->sensorList[$s]->state = $action;
	  }
      }
    
    $this->saveSensorState();  
  }

  public function combineImagesOnValidSensors()
  {
    $result = array();
 
    if(count($this->validSensorList) > 0)
      {
	$cmdline = BQBH_PROCESSOR_BIN . " -m 4 -p " . $this->serverPath();    
	foreach($this->validSensorList as $s)
	  {
	    $id = $this->sensorList[$s]->sensorRef->id;
	    $clientId = $this->sensorList[$s]->sensorRef->clientId;
	    $proj_img_name = $this->projImageName($this->sensorList[$s]->sensorRef);
	    
	    $cmdline .= " -s $clientId -k $id -i $proj_img_name ";
	  }	
	exec($cmdline, $output);
	
	$resultlines = preg_grep('/CONVERT=/', $output);

	$result = array();
	$zone = 0;
	foreach($resultlines as $l)
	  {
	    $IDX=666;
	    $CONVERT=666;
	    $RAW=666;	    
	    $REGION=0;

	    $l = preg_replace('/IDX=/', '$IDX=', $l);
	    $l = preg_replace('/REGION=/', '$REGION=', $l);	    
	    $l = preg_replace('/RAW=/', '$RAW=', $l);	    
	    $l = preg_replace('/CONVERT=/', '$CONVERT=', $l);
	    $l = preg_replace('/$/', ';', $l);
	   
	    eval($l);

	    $idx = $IDX;
	    $zone = $REGION;
	    $raw = $RAW;
	    $value = $CONVERT;
	    
	    $result[$zone] = array('idx' => $idx,
				   'raw' => $raw,
				   'value' => $value);
	    $zone++; 
	  }
      }

    if(count($result) == 0)
      {
	$result[0] = array('raw' => -1,
			   'value' => -1);
      }

    return $result;
  }

  public function saveResult($result)
  {
    $handle = fopen($this->serverPath() . "/result.txt", "w");
    $handleHistory = fopen($this->serverPath() . "/resultHistory.txt", "w");
    $handleTmp = fopen($this->tmpDir() . "/result.txt", "a");
    $handleTime = fopen($this->serverPath() . "/timer.txt", "w");

    if($handleTime)
      {
	fwrite($handleTime, $this->getTimer());
	fclose($handleTime);
      }

    foreach($result as $zone => $res)
      {
	$resstr = "$zone " . $this->currentProcessingKey . 
	  " " . $res['raw'] . " " . $res['value'] . "\n";
	
	if($handle)
	  {
	    fwrite($handle, "$resstr");
	  }

	if($handleTmp)
	  {
	    fwrite($handleTmp, "$resstr");
	  }
	
	if($handleHistory)
	  {
	    fwrite($handleHistory, "$resstr");
	  }
      }

    if($handle)
      {
	fclose($handle);
      }

    if($handleHistory)
      {
	fclose($handleHistory);
      }
    
    if($handleTmp)
      {
	fclose($handleTmp);
      }
  }
  
  public function buildPageUsedByValidationRecording($result)
  {
    $tmpdir = $this->tmpDir();
    // prepare web page for validation recording
    $handle = fopen($this->serverPath() . "/result_for_validation.html", "w");
    if($handle)
      {
	fwrite($handle, "<html>\n <body>\n");
	
	foreach($result as $zone => $res)
	  {
	    fwrite($handle, "VAR_ZONE=$zone\n");
	    fwrite($handle, "VAR_TIMESTAMP=" . $this->currentProcessingKey . "\n");
	    fwrite($handle, "VAR_VALUE=" . $res['value'] . "\n");
	    fwrite($handle, "VAR_RAWVALUE=" . $res['raw'] . "\n");
	  }
	
	$dir = opendir($tmpdir);
	
	if($dir)
	  {
	    $wwwdir = $this->tmpURL();
	    while($str = readdir($dir))
	      {
		if(preg_match('/img_.*\.jpg/', $str))
		  {
		    fwrite($handle, "<img src=\"$wwwdir/$str\"/>\n");
		  }
	      }
	    closedir($dir);
	  }
	fwrite($handle, " </body>\n</html>\n");
	fclose($handle);
      }
  }
  
  public function process($timestamp)
  {
    $this->startTimer();

    $this->currentProcessingKey = $timestamp;
    Common_createAndRotateTmpDir(BQBHSERVER_ROTATE_MAX, $this->serverPath(), $this->currentProcessingKey);
    
    // lock
    $this->processLock();
    
    // initialize
    $this->createSensorList();
   
    foreach($this->sensorList as $s => $r)
      {
	$this->sensorList[$s]->state = "X";
	$this->sensorList[$s]->currentProcessingKey = $this->currentProcessingKey;
      }
    
    // start processing on each sensor
    $this->doActionOnDistantSensors("updateConfig");
     
    $this->doActionOnDistantSensors("grabImage");

    // retrieve grabbed image (validation purpose)    
    echo "Step: retrieve grabbed image\n";
    $this->updateValidSensorList();
    foreach($this->validSensorList as $s)
      {
	  $params = array('imgname' => "img.jpg",
			  'outputfile' => $this->imageName($this->sensorList[$s]->sensorRef));
	  $this->sensorList[$s]->retrieveProcessingFile($params);
      }

    // image processing (local computation)
    $this->doActionOnDistantSensors("imageProcessing");
    
    // retrieve result images
    echo "Step : retrieve result image\n";
    $this->updateValidSensorList();

   foreach($this->validSensorList as $s)
      {
	  $params = array('imgname' => "proj.ppm",
			  'outputfile' => $this->projImageName($this->sensorList[$s]->sensorRef));
	  $this->sensorList[$s]->retrieveProcessingFile($params); 
      }


    echo "Step : combination\n";    
    $result = $this->combineImagesOnValidSensors();

    $this->endTimer();
    
    echo "Step : Save result\n";

    $this->saveResult($result);
    
    $this->buildPageUsedByValidationRecording($result);


    // unlock
    $this->processUnlock();
  
    $this->currentProcessingKey = "0000";
  }

  protected function startTimer()
  {
    $time = explode(" ", microtime());
    $this->startTime = $time[1] + $time[0];
  }

  protected function endTimer()
  {
    $time = explode(" ", microtime());
    $this->endTime = $time[1] + $time[0];
  }

  public function getTimer()
  {
    return round($this->endTime - $this->startTime, 1);
  }
}

?>
