<?php

include_once("BluePHP/System/System.inc");
include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/Utils/ModuleProcessing.inc");

class Date extends ModuleProcessing
{
  private $sys = null;

  public function __construct($sharedCon = null)
  {
    parent::__construct(array());
    $this->sys = new System();
  }

  function getEntry($params)
  {
    $params = array_merge( $this->getRdateParams(),
			   $this->getNTPParams(),
			   $this->getLocalTimeParams() );
    $params["DATE_timeMethod"] = "local";
    if($params["DATE_useNTP"] != "0")
      {
	$params["DATE_timeMethod"] = "ntp";
      }
    else if($params["DATE_useRdate"] == "true")
      {
	$params["DATE_timeMethod"] = "time";
      }
    return $params;
  }

  function setEntry($params)
  {
    if(isset($params["DATE_timeMethod"]))
      {
	if($params["DATE_timeMethod"] == "local")
	  {
	    $params["DATE_useRdate"] = "false";
	    $params["DATE_useNTP"] = "false";
	    $this->setLocalTime($params);
	  }
	else if($params["DATE_timeMethod"] == "ntp")
	  {
	    $params["DATE_useRdate"] = "false";
	    $params["DATE_useNTP"] = "on";
	  }
	else if($params["DATE_timeMethod"] == "time")
	  {
	    $params["DATE_useRdate"] = "true";
	    $params["DATE_useNTP"] = "false";
	  }
	$this->setNTPValues($params);
	$this->setRdateValues($params);
      }
    $this->setTimeZoneValues($params);
    return $params;
  }

  public function getLocalTimeParams()
  {
    $params = $this->getTimeZoneParams();
    date_default_timezone_set($params["DATE_TIMEZONE"]);
    $params["DATE_localTime"] = strftime("%Y/%m/%d %H:%M:%S");
    return $params;
  }

  public function setLocalTime($params)
  {
    if(isset($params["DATE_localTime"]))
      {
	$localTime = $params["DATE_localTime"];
	$str = "source /etc/BEV/scripts/BlueSystem/Date.inc\n" . 
	  "DATE_MONTH=" . substr($localTime, 5, 2) . "\n" . 
	  "DATE_DAY=" . substr($localTime, 8, 2) . "\n" . 
	  "DATE_HOUR=" . substr($localTime, 11, 2) . "\n" . 
	  "DATE_MINUTE=" . substr($localTime, 14, 2) . "\n" . 
	  "DATE_YEAR=" . substr($localTime, 0, 4) . "\n" . 
	  "DATE_SECOND=00\n" . 
	  "Date_setDateValues";
	exec($str);
      }
    return $params;
  }

  public function getTimeZoneParams()
  {
    $str = "source /etc/BEV/scripts/BlueSystem/Date.inc ; Date_getTimeZoneValues ; " . 
      'echo -e "DATE_TIMEZONE=$DATE_TIMEZONE"';
    $output = array();
    exec($str, $output);
    return parseSimplePairValues($output);
  }

  public function setTimeZoneValues($params)
  {
    if(isset($params["DATE_TIMEZONE"]))
      {
	$str = "source /etc/BEV/scripts/BlueSystem/Date.inc\n" . 
	  "DATE_TIMEZONE=" . $params["DATE_TIMEZONE"] . "\n" . 
	  "Date_setTimeZoneValues";
	exec($str);
      }
    return $params;
  }

  public function getRdateParams()
  {
    $str = "source /etc/BEV/scripts/BlueSystem/Date.inc ; Date_getRdateValue ; " . 
      'echo -e "DATE_hasRdate=$DATE_hasRdate\nDATE_useRdate=$DATE_useRdate\n' . 
      'DATE_RdateServer=$DATE_RdateServer"';
    $output = array();
    exec($str, $output);
    return parseSimplePairValues($output);
  }

  public function setRdateValues($params)
  {
    $params = array_merge($this->getRdateParams(), $params);
    $str = "source /etc/BEV/scripts/BlueSystem/Date.inc\n" . 
      "DATE_hasRdate=" . $params["DATE_hasRdate"] . "\n" . 
      "DATE_useRdate=" . $params["DATE_useRdate"] . "\n" . 
      "DATE_RdateServer=" . $params["DATE_RdateServer"] . "\n" .
      "Date_setRdateValue";
    exec($str);
    return $params;
  }

  public function getNTPParams()
  {
    $str = "source /etc/BEV/scripts/BlueSystem/Date.inc ; Date_getNTPValues ; " . 
      'echo -e "DATE_hasNTP=$DATE_hasNTP\nDATE_useNTP=$DATE_useNTP\n' . 
      'DATE_NTPServer=$DATE_NTPServer\nDATE_isTimeServer=$DATE_isTimeServer"';
    $output = array();
    exec($str, $output);
    return parseSimplePairValues($output);
  }

  public function setNTPValues($params)
  {
    $params = array_merge($this->getNTPParams(), $params);
    $str = "source /etc/BEV/scripts/BlueSystem/Date.inc\n" . 
      "DATE_hasNTP=" . $params["DATE_hasNTP"] . "\n" . 
      "DATE_NTPServer=" . $params["DATE_NTPServer"] . "\n" . 
      "DATE_isTimeServer=" . $params["DATE_isTimeServer"] . "\n" .
      "DATE_useNTP=" . $params["DATE_useNTP"] . "\n" . 
      "Date_setNTPValues";
    exec($str);
    //file_put_contents("/tmp/toto", $str);
    return $params;
  }

};


?>