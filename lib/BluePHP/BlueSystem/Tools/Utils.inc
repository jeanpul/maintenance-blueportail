<?php

function Utils_timestamp()
{
  return strftime("%Y%m%d%H%M%S", time());
}

// delete file, and if directory : delete content recursively
// before removing directory
function Utils_recursiveDelete($file)
{
  if(file_exists($file))
    {
      if(is_dir($file))
	{
	  
	  $handle = opendir($file);
	  if($handle)
	    {
	      while($str = readdir($handle))
		{
		  if($str != "." && $str != "..")
		    {
		      Utils_recursiveDelete($file . "/" . $str);
		    }
		}
	      closedir($handle);
	    }
	  rmdir($file);	 
	}
      else
	{
	  unlink($file);
	}
    }
}

// check if all values defined in $keys
// are existing key in array
// return the missing keys
function Utils_checkKeysInArray($keys, $array)
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

function Utils_downloadFile($url, $destfile, $timeout = 10)
{
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

