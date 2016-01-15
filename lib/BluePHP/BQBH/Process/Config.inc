<?php

/**
Name : 
Constant for BQBHServer side : start with BQBHSERVER_
Constant fr BQBHSensor side : start with BQBHSENSOR_
Constant used by the two side : start with BQBH_
*/

define('BQBHSERVER_LICENSE_FILENAME', "license_bqbhserver.txt");
define('BQBHSERVER_LICENSE_FILE', "/etc/BEV/modules/BQBH/" . BQBHSERVER_LICENSE_FILENAME);

define('BQBHSERVER_PATH', "/home/DATA/BQBHServer/");
define('BQBHSERVER_WWW_PATH', "/BQBH/BQBHServer/");

define('BQBHSENSOR_PATH', "/home/DATA/BQBH/");
define('BQBHSENSOR_WWW_PATH', "/BQBH/BQBH/");

define('BQBHSERVER_ROTATE_MAX', 1);
define('BQBHSENSOR_ROTATE_MAX', 1);

define('BQBHSERVER_SENSOR_REQUEST_TIMEOUT', 59);


define('BQBH_PROCESSOR_BIN', "/usr/bin/BQBH/processor -v");

define('BQBHSERVER_DISTANT_SENSOR_ACTION_BIN', "/BQBH/BQBHAction.php");

define('BQBHSERVER_ACTION_EXEC_BIN', "/usr/bin/BQBH/doActionOnSensor.php");
define('BQBHSERVER_CREATE_CONFIG_ARCHIVE_BIN', "/usr/bin/BQBH/createConfigurationArchive.sh");

define('BQBH_ERROR_NO_SENSOR_ANSWER', 50);
define('BQBH_ERROR_INCOMPLETE_SENSOR_ANSWER', 51);
define('BQBH_ERROR_SENSOR_MISSING_COMMON_ARG', 30);
define('BQBH_ERROR_SENSOR_ACTION_EXCEPTION', 31);
define('BQBH_ERROR_SENSOR_MISSING_SPECIFIC_ARG', 32);
define('BQBH_ERROR_SENSOR_MISSING_SOURCE_FILE', 33);
define('BQBH_ERROR_SENSOR_DEST_FILE_NOT_CREATED', 34);
define('BQBH_ERROR_SENSOR_INCORRECT_IDENTIFICATION', 35);
define('BQBH_ERROR_SENSOR_CONFIG_DOWNLOAD_FAILED', 36);
define('BQBH_ERROR_SENSOR_CONFIG_DOWNLOAD_INCORRECT', 37);
define('BQBH_ERROR_SENSOR_GRAB_ERROR', 38);
define('BQBH_ERROR_SENSOR_PROCESSOR_ERROR', 39);
define('BQBH_ERROR_SENSOR_MISSING_IDENTIFICATION_ELEMENT', 40);


define('BQBH_GRAB_BIN', '/usr/bin/BQBH/grab.sh');
define('BQBH_CONFIG_PATH', "/etc/BEV/config/BQBH/");
define('BQBH_BLUEEYEVIDEORC', BQBH_CONFIG_PATH . "/configrc.xml");

function Logs_start($module)
{
  if(!is_dir("/var/log/bev"))
    {
      openlog($module . " : ", LOG_PID, LOG_USER);
    }
}

function Logs_end()
{
  if(!is_dir("/var/log/bev"))
    {
      closelog();
    }
}

function Logs_print_generic($module, $type, $message)
{
  if(is_dir("/var/log/bev"))
    {
      $line = strftime("%Y%m%d%H%M%S") . " : " . $module . " : " . $type . " : " . $message . "\n";
      file_put_contents("/var/log/bev/${module}_${type}.log", $line, FILE_APPEND | LOCK_EX);
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

function Logs_info($module, $message)
{
  Logs_print_generic($module, "info", $message);
}

function Logs_warning($module, $message)
{
  Logs_print_generic($module, "warning", $message);
}

function Logs_error($module, $message)
{
  Logs_print_generic($module, "error", $message);
}

?>
