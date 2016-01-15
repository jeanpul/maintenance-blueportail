<?php

include_once('BlueSystemToolsPHP/DistantProcessingElementConstant.inc');

class DistantProcessingElement
{

  private $lastActionRessource;
  private $lastActionEndStatus;
  
  public function askAction($dsthost, $distant_action_urlpart, $request_params, $timeout)
  {
    $this->lastActionRessource = -1;
    $this->lastActionEndStatus = -1;
     
    $urlparam = "";
    foreach($request_params as $n => $v)
      {
	if($urlparam != "") $urlparam .= "&";
	$urlparam .= $n . "=" . $v;
      }
    
    $str = $distant_action_urlpart . "?" . $urlparam;
    
    $cmd = DISTANT_ACTION_EXEC_BIN . " " . $dsthost . " " .$timeout . " \"$str\"";
    echo "DPE:askAction ". $cmd . "\n";
    
    $this->lastActionRessource = popen($cmd, "r");
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
  
  public function getLastActionStatus()
  {
    return $this->lastActionEndStatus;
  }
};

?>