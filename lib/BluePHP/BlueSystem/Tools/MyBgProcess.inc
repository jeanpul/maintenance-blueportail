
<?php

/**
 * \class MyBgProcess
 * To run a background process
 */
class MyBgProcess
{
  /**
   * process pid
   */
  public $pid;

  /**
   * Create or attach a new process
   * \param $pid attach to an existing process or -1
   */
  public function __construct($pid=-1)
  {
    $this->pid = $pid;
  }
  
  /**
   * Start the process in background
   * \param $cmd process command
   */
  public function startInBg($cmd)
  {
    $command = 'nohup '.$cmd . ' > /dev/null 2>&1 & echo $!';
    exec($command , $op);
    $this->pid = (int)$op[0];
  }
  
  /**
   * Return the process status
   */
  public function status()
  {
    $command = 'ps -p '.$this->pid;
    exec($command,$op);
    //var_dump($op);
    if (!isset($op[1]))return false;
    else return true;
  }

  /**
   * Kill the process
   * \param $signal signal to send
   */
  public function stop($signal = -1)
  {
    $command = 'kill ';
    if($signal != -1) 
      {
	$command .= '-' . $signal . ' ';
      }
    $command .= $this->pid;
    exec($command);
    if ($this->status() == false)return true;
    else return false;
  }

};

?>
