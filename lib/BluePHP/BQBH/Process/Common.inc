<?php

include_once('BluePHP/BlueSystem/Tools/BSbasetools.inc');

function Common_timestamp()
{
  return strftime("%Y%m%d%H%M%S", time());
}

function Common_tmpDir($basepath, $timestamp)
{
  return $basepath . "tmp_" . $timestamp . "/";
}

function Common_serverDirOnServerSide($basepath, $serverRef)
{
  return $basepath . "/" . $serverRef->getServerName() . "/";
}

function Common_sensorDirOnServerSide($basepath, $sensorRef)
{
  return Common_serverDirOnServerSide($basepath, $sensorRef->serverRef) .
    $sensorRef->getSensorName() . "/";
}

function Common_serverDirOnSensorSide($basepath, $serverRef)
{
  return $basepath . "/server_" . $serverRef->clientId . "_" . $serverRef->id . "/";
}

function Common_sensorDirOnSensorSide($basepath, $sensorRef)
{
  return Common_serverDirOnSensorSide($basepath, $sensorRef->serverRef) .
    $sensorRef->getSensorName() . "/";
}

function Common_tmpDirOnSensorSide($basepath, $sensorRef, $timestamp)
{
  $p = Common_sensorDirOnSensorSide($basepath, $sensorRef);
  return Common_tmpDir($p, $timestamp);  
}


/**
 * delete a file
 * if the file is a directory : delete recusively the content and delete it
 */
function Common_recursiveDeleteFiles($file)
{
  if(!is_dir($file))
    {
      unlink($file);
    }
  else
    {
      $handle = opendir($file);
      if($handle)
	{
	  while($str = readdir($handle))
	    {
	      if($str != "." && $str != "..")
		{
		  Common_recursiveDeleteFiles($file . "/" . $str);
		}
	    }
	  closedir($handle);
	}
      rmdir($file);
    }
}

function Common_tmpDirListFile($basepath)
{
    return $basepath . "/tmp.lst";
}

function Common_readTmpDirList($tmplistfile)
{
    $tmplist = array();
    if(file_exists($tmplistfile))
    {
      $handle = fopen($tmplistfile, "r");  
      if($handle)
	{     
	  while(!feof($handle))
	    {
	      $line = fgets($handle);
	      if($line != "")
		{		  
		  array_push($tmplist, preg_replace('/\n/', '', $line));
		}
	    }
	  fclose($handle);
	}
    }
    return $tmplist;
}

function Common_createAndRotateTmpDir($max, $basepath, $timestamp)
{  
  //rotate
  $tmplistfile = common_tmpDirListFile($basepath);

  //read file
  $tmplist = Common_readTmpDirList($tmplistfile);
  
  // remove oldest directory if more than $max
  if(count($tmplist) > $max)
    {
      // substring on tmplist[0] to remove \n at the end
      Common_recursiveDeleteFiles($tmplist[0]);
      array_shift($tmplist);
    }
  
  // create the new directory
  $tmpdir = Common_tmpDir($basepath, $timestamp);
  mkdir($tmpdir);
  
  // add the new directory and regenerate the complete list
  array_push($tmplist, $tmpdir);
  $handle = fopen($tmplistfile, "w");
  if($handle)
    {
      foreach($tmplist as $e)
	{
	  fwrite($handle, $e . "\n");
	}
      fclose($handle);
    }
  else 
    {
      throw new Exception("Cannot open file : $tmplistfile \n");
    }
}

// check if all values defined in $keys
// are existing key in array
// return the missing keys
function Common_checkKeysInArray($keys, $array)
{
  $missing = array();
  foreach($keys as $k)
    {
      if(!array_key_exists($k, $array))
	{
	  array_push($missing, $k);
	}
    }
  return $missing;
}

function Common_obtainHostClientId()
{
  return BSbasetools_obtainHostClientId();
}

function Common_downloadFile($url, $destfile, $timeout = 10)
{
  $ret = 0;
  $dest = fopen($destfile, "wb");
  if(!$dest)
    {
      return 2;
    }
  else
    {
      $handle = curl_init();
      curl_setopt($handle, CURLOPT_URL, $url);
      curl_setopt($handle, CURLOPT_HEADER, false);
      curl_setopt($handle, CURLOPT_TIMEOUT, $timeout);
      curl_setopt($handle, CURLOPT_FILE, $dest);
      
      if(!curl_exec($handle))
	{
	  echo "Download file error : " . curl_error($handle) ."\n";
	  $ret = 1;
	} 
      
      curl_close($handle);
      fclose($dest);
    }
  return $ret;
}


function Common_errorCodeToStr($err)
{
  switch($err)
    {
    case BQBH_ERROR_NO_SENSOR_ANSWER:
      return _("No sensor answer.");
    case BQBH_ERROR_INCOMPLETE_SENSOR_ANSWER:
      return _("Incomplete sensor answer.");
    case BQBH_ERROR_SENSOR_MISSING_COMMON_ARG:
      return _("Missing common argument in request to sensor.");
    case BQBH_ERROR_SENSOR_ACTION_EXCEPTION:
      return _("Exception in sensor action.");
    case BQBH_ERROR_SENSOR_MISSING_SPECIFIC_ARG:
      return _("Missing specific argument in request to sensor.");
    case BQBH_ERROR_SENSOR_MISSING_SOURCE_FILE:
      return _("Missing source file in manipulation.");
    case BQBH_ERROR_SENSOR_DEST_FILE_NOT_CREATED:
      return _("Output file not created.");
    case BQBH_ERROR_SENSOR_INCORRECT_IDENTIFICATION:
      return _("Incorrect identification of sensor.");
    case BQBH_ERROR_SENSOR_CONFIG_DOWNLOAD_FAILED:
      return _("Download of configuration failed.");
    case BQBH_ERROR_SENSOR_CONFIG_DOWNLOAD_INCORRECT:
      return _("Download incorrect of configuration");
    case BQBH_ERROR_SENSOR_GRAB_ERROR:
      return _("Grabber tool error.");
    case BQBH_ERROR_SENSOR_PROCESSOR_ERROR:
      return _("Processing tools error.");
    case BQBH_ERROR_SENSOR_MISSING_IDENTIFICATION_ELEMENT:
      return _("Missing element to check sensor reference.");
    default:
      $msg = _("No message error for %d.");
      return sprintf($msg, $err);
    }
}

?>
