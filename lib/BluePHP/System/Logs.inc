<?php

/**
 * Provides a logging mechanism per module,
 * each module use a different log file in the format :
 * <log_root_dir>/<module_name>_<info|warning|error>.log
 * Use common syslog in case of error
 */
class Logs
{
  private $module = null;
  private $syslogUsed = false;
  private $log_root_dir = null;

  function __construct($module, $log_root_dir = "/var/log/bev")
  {
    $this->module = $module;
    $this->log_root_dir = $log_root_dir;
    if(!is_dir($log_root_dir))
      {
	$this->init_syslog();
      }
  }

  function __destruct() 
  {
    if($this->syslogUsed)
      {
	closelog();
      }
  }

  public function info($message)
  {
    $this->logs_print_generic("info", $message);
  }

  public function warning($message)
  {
    $this->logs_print_generic("warning", $message);
  }

  public function error($message)
  {
    $this->logs_print_generic("error", $message);
  }

  public function isSyslogUsed()
  {
    return $this->syslogUsed;
  }

  protected function logs_print_generic($type, $message)
  {
    if(!$this->syslogUsed)
    {
      $line = strftime("%Y%m%d%H%M%S") . " : " . $this->module . " : " . $type . " : " . $message . "\n";
      if(!file_put_contents($this->log_root_dir . "/" . $this->module . "_" . $type . ".log", $line, FILE_APPEND | LOCK_EX))
	{ // in case of errors switch back to syslog
	  $this->init_syslog();
	}
    }
  else
    {
      $systype = LOG_INFO;
      if($type == "warning")
	{
	  $systype = LOG_WARNING;
	}
      else if($type == "error")
	{
	  $systype = LOG_ERR;
	}
      syslog($systype, $message);
    }
  }

  private function init_syslog()
  {
    openlog($this->module . " : ", LOG_PID, LOG_USER);
    $this->syslogUsed = true;
  }
}

?>
