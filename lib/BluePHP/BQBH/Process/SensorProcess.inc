<?php

/**
 * Do processing on sensor host side
 */
class SensorProcess
{
  public $basePath;
  public $sensorRef;
  
  public function __construct($sensorRef)
  {
    $this->basePath = BQBHSENSOR_PATH;
    $this->sensorRef = $sensorRef;

    // create server directory for this sensor if not exist
    if(!file_exists($this->serverDirOnSensorSide()))
      {	
	mkdir($this->serverDirOnSensorSide());
      }
    
    if(!file_exists($this->sensorDirOnSensorSide()))
      {
	mkdir($this->sensorDirOnSensorSide());
      }
  }
  
  public function doAction($params)
  {
    $method = "do_" . $params["action"];
    try 
      {
	$result = $this->$method($params);
      }
    catch(Exception $e)
      {	
	$result = $this->errorResult(BQBH_ERROR_SENSOR_ACTION_EXCEPTION,
				     $e->getMessage());
      }
    return $result;
  }
  
  // -- SECTION : Auxiliary method for action execution --
  
  private function sensorDirOnSensorSide()
  {
    return Common_sensorDirOnSensorSide($this->basePath, $this->sensorRef);
  }

  private function serverDirOnSensorSide()
  {
    return Common_serverDirOnSensorSide($this->basePath, $this->sensorRef->serverRef);
  }

  private function tmpDir($processingKey)
  {
    return Common_tmpDirOnSensorSide($this->basePath, $this->sensorRef, $processingKey);
  }


  
  private function errorResult($error_number, $error_msg)
  {
    return array("status" => $error_number,
		 "output" => array($error_msg));
  }

  private function okResult($msg = "")
  {
    return array("status" => 0,
		 "output" => array($msg));
  }

  // test sensor availability
  private function checkSensorRef()
  {    
    // check Client id
    $host_clientid = Common_obtainHostClientId();
   
    if($this->sensorRef->clientId != $host_clientid)

      {
	return $this->errorResult(BQBH_ERROR_SENSOR_INCORRECT_IDENTIFICATION,
				  "Client Id do not match ([" . 
				  $this->sensorRef->clientId .
				  "] - [" . $host_clientid . "])");
      }
    // check id 

    $handle = fopen(BQBH_BLUEEYEVIDEORC, "r");
    if($handle)
      {
	$found = false;
	while(!feof($handle) && !$found)
	  {
	    
	    $line = fgets($handle);
	    if(preg_match('/<channel .*id=/', $line))
	      {
		$id=preg_replace('/.*channel.* id=\"([0-9]*)\".*\n/', '$1', $line);
		$found = ($id == $this->sensorRef->id);
	      }
	  }
	fclose($handle);
	
	if(! $found)
	  {
	    return $this->errorResult(BQBH_ERROR_SENSOR_INCORRECT_IDENTIFICATION, 
				      "B-Queue sensor with id (" . $this->sensorRef->id . ") not available");
	  }
	
      }
    else
      {
	return $this->errorResult(BQBH_ERROR_SENSOR_MISSING_IDENTIFICATION_ELEMENT, 
				  "Id (" . $this->sensorRef->id . ") not available");
      }
    return false;
  }

  private function checkParameters($paramnames, $params)
  {
    $missing = Common_checkKeysInArray($paramnames, $params);
    if(count($missing) > 0)
      {
	$msg = "";
	foreach($missing as $e)
	  {
	    $msg .= "$e,";
	  }
	return $this->errorResult(BQBH_ERROR_SENSOR_MISSING_SPECIFIC_ARG,
				  "Missing parameters : $msg");
      }
    return false;
  }
  
  private function checkFiles($files, $error_number, $msg)
  {
    foreach($files as $f)
      {
	if(!file_exists($f))
	  {
	    return $this->errorResult($error_number,
				      "$msg : $f");
	  }
      }
    return false;
  }
  
  private function checkSourceFiles($files)
  {
    return $this->checkFiles($files, BQBH_ERROR_SENSOR_MISSING_SOURCE_FILE, "Missing file");
  }
  
  private function checkCreatedFiles($files)
  {
    return $this->checkFiles($files, BQBH_ERROR_SENSOR_DEST_FILE_NOT_CREATED, "File not created");
  }
  
  private function sourceImage($processingKey)
  {
    return $this->tmpDir($processingKey) . "img.jpg";
  }
  
  private function projImage($processingKey)
  {
    return $this->tmpDir($processingKey) . "proj.ppm";
  }
  
  private function configArchive()
  {
    return $this->sensorDirOnSensorSide() . "config.tgz";
  }

  private function configDir()
  {
    return $this->sensorDirOnSensorSide() . "config";
  }
  
  // -- SECTION : Available action --
  
  public function do_updateConfig($params)
  {
    if($result = $this->checkSensorRef())
      {
	return $result;
      }
    
    $neededParams = array("processingKey", "md5");
    if($result = $this->checkParameters($neededParams, $params))
      {
	return $result;
      }

    Common_createAndRotateTmpDir(BQBHSENSOR_ROTATE_MAX, 
				 $this->sensorDirOnSensorSide(), 
				 $params["processingKey"]);
    $config_archive = $this->configArchive();
    
    if(file_exists($config_archive))
      {
	$md5 = md5_file($config_archive);
	if($md5 == $params['md5'])
	  {
	    return $this->okResult("Configuration up-to-date.");
	  }
      }

    // load the new configuration
    $url = "http://" . $params['remote_ip'] . 
      Common_sensorDirOnServerSide(BQBHSERVER_WWW_PATH, $this->sensorRef) . 
      "config.tgz";
    $ret = Common_downloadFile($url, $config_archive, BQBHSERVER_SENSOR_REQUEST_TIMEOUT);

    if($ret != 0)
      {
	return $this->errorResult(BQBH_ERROR_SENSOR_CONFIG_DOWNLOAD_FAILED, 
				  "Failed to download update config archive [$url]");
      }

    $md5 = md5_file($config_archive);
    if($md5 != $params['md5'])
      {
	return $this->errorResult(BQBH_ERROR_SENSOR_CONFIG_DOWNLOAD_INCORRECT, 
				  "MD5 of config archive do not match ($md5 - " . $params['md5'] . ")");
      }

    //install config
    $config_dir = $this->configDir();
    if(file_exists($config_dir))
    {
	Common_recursiveDeleteFiles($config_dir);
    }
        
    $cmd = "tar zxv -C " . $this->sensorDirOnSensorSide() . " -f " . $config_archive;

    exec($cmd);

    $this->checkCreatedFiles(array($config_dir));
    
    return $this->okResult("Configuration uploaded and updated");
  }
  
  public function do_grabImage($params)
  {
    if($result = $this->checkSensorRef())
      {
	return $result;
      }
    
    if($result = $this->checkParameters(array("processingKey"), $params))
      {
	return $result;
      }
    
    $processingKey = $params["processingKey"];
    
    $dest = $this->sourceImage($processingKey);        

    $cmdline = BQBH_GRAB_BIN . " " . $this->sensorRef->id . " $dest";

    $ret = 666;
    exec($cmdline, $output, $ret);    

    if($ret != 0)
      {
	
	  array_push($output, "Exit code = $ret");
	  return array('status' => BQBH_ERROR_SENSOR_GRAB_ERROR, 
		       'output' => $output);
      }

    if($result = $this->checkCreatedFiles(array($dest)))
      {
	$result['output'] = array_merge($output, $result['output']);
	return $result;
      }

      /**    if(file_exists("/var/www/statussnap/"))
      {
	copy($dest, "/var/www/statussnap/". $this->sensorRef->id .".jpg");
      } */

    array_push($output, "Image recorded");
    return array('state' => 0,
		 'output' => $output);
  }
  
  public function do_imageProcessing($params)
  {    
    if($result = $this->checkSensorRef())
      {
	return $result;
      }
    
    if($result = $this->checkParameters(array("processingKey"), $params))
      {
	return $result;
      }
   

    $processingKey = $params["processingKey"];
    $tmpdir = $this->tmpDir($processingKey);

    $tmpbg = $this->sensorDirOnSensorSide() . "tmpbg.bin";
    $tmptmpbg = $tmpdir . "tmpbg.bin";
    $src = $this->sourceImage($processingKey);
    $dest = $this->projImage($processingKey);

    if($result = $this->checkSourceFiles(array($src)))
      {
	return $result;
      }
    
    if(file_exists($tmpbg))
      {
        copy($tmpbg, $tmptmpbg);
      }

    $cmdline = BQBH_PROCESSOR_BIN . " -m 3 -p " . 
	$this->serverDirOnSensorSide() . 
	" -s " . $this->sensorRef->clientId . 
        " -k " . $this->sensorRef->id . 
	" -i " . $src .
	" -o " . $dest;

    // Do processing in tmp directory
    // tmpbg saved in this directory
    chdir($tmpdir);

    $ret = 666;
    exec($cmdline, $output, $ret);

    if(file_exists($tmptmpbg))
      {
        copy($tmptmpbg, $tmpbg);
      }

    array_push($output, "CMD=$cmdline");      
    if($ret != 0)
    {
	array_push($output, 'Processing Error : Exit code = $ret');
	return array('status' => BQBH_ERROR_SENSOR_PROCESSOR_ERROR,
		     'output' => $output);
    }
    else
      {
	array_push($output, "Processing Exit code = 0");
      }

    if($result = $this->checkCreatedFiles(array($dest)))
      {
	  $result['output'] = array_merge($output, $result['output']);
	  return $result;
      }

    array_push($output, 'Image processed');
    return array('status' => 0,
		 'output' => $output);
  }
}
  
?>
