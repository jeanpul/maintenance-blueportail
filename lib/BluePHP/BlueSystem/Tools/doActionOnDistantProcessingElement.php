#!/usr/bin/php
<?php

/**
- dest host
- timeout (sec)
- URL part
 */

include_once('BluePHP/BlueSystem/Tools/DistantProcessingElementConstant.inc');

$ret = 0;

print_r($argv);

$ip = $argv[1];
$timeout = $argv[2];
$urlpart = $argv[3];

$handle = curl_init();


$url = "http://" . $ip . $urlpart;

echo "URL=[$url]\n";

curl_setopt($handle, CURLOPT_URL, $url);
curl_setopt($handle, CURLOPT_HEADER, false);
curl_setopt($handle, CURLOPT_TIMEOUT, $timeout);
curl_setopt($handle, CURLOPT_RETURNTRANSFER, true);
curl_setopt($handle, CURLOPT_FAILONERROR, true);

$output = curl_exec($handle);

if($output)
{  
  $input = preg_split('/\n/', $output);
  $found = preg_grep('/STATUS=/', $input);

  if(count($found))
    {
      echo $output;
      
      $status = array_pop($found);
      
      $ret = preg_replace('/.*STATUS=(\d+).*/i', '${1}', $status);
    }
  else
    {      
      foreach($input as $r)
	{
	  echo 'STATUS_MESSAGE="' . $r . '"' . "\n";
	}
      
      $ret = DISTANT_PROCESSING_ELEMENT_INCOMPLETE_ANSWER;
      
      echo "STATUS=" . $ret . "\n";
    }
}
else
{
  echo "NO OUTPUT : " . curl_error($handle) ."\n";
  $ret = DISTANT_PROCESSING_ELEMENT_NO_ANSWER;
  echo "STATUS=" . $ret . "\n";
}

curl_close($handle);
$ret = (int)$ret;
echo "Exit status : [$ret]\n";

exit($ret);

?>
