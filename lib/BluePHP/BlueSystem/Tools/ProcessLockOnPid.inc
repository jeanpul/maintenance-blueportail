<?php

class ProcessLockOnPid
{
  public $lockfile;

  public function ProcessLockOnPid($lockfile)
  {
    $this->lockfile = $lockfile;
  }
  
  private function testPid($pid)
  {
    $command = 'ps -p '.$pid;
    exec($command,$op);   
    if (isset($op[1])) return true;
    else return false;
  }

  public function lockStatus()
  {
    if(file_exists($this->lockfile))
      {
	$valid = false;
	$handle = fopen($this->lockfile, 'r');
	if($handle)
	  {
	    $pid = fread($handle, filesize($this->lockfile));
	    if($this->testPid($pid))
	      {
		$valid = true;
	      }
	    fclose($handle);
	  }

	if(!$valid)	
	  {
	    unlink($this->lockfile);
	  }
	else
	  {
	    return true;
	  }
      }
    return false;
  }
  
  public function processLock($pid)
  {
    
    if($this->lockStatus())
      {
	return false;
      }
    else
      {
	$handle = fopen($this->lockfile, 'w');
	if($handle)
	  {
	    fwrite($handle, $pid);
	    fclose($handle);
	  }
	return true;
      }
  }

  public function processUnlock()
  {
    if(file_exists($this->lockfile))
      {
	unlink($this->lockfile);
      }
  }
  
};

?>