<?php

include_once("BluePHP/Utils/ModuleProcessing.inc");
include_once("BluePHP/Utils/DateOps.inc");
include_once("BluePHP/BTopLocalServer/Config.inc");
include_once("BluePHP/BTopLocalServer/BlueCountLang.inc");
include_once("BluePHP/BTopLocalServer/BTopSensors.inc");

class GlobalProperties extends ModuleProcessing
{
  private $clang = false;

  public function __construct($sharedCon = null)
  {
    parent::setParams( array( "name" => "GlobalProperties",
			      "keyname" => "name",
			      "sort" => "name" ),
		       $sharedCon);
    $this->clang = new BlueCountLang();
  }

  public function getLastPushTask()
  {
    return parent::Select("* from GlobalProperties where name='lastPushTask'");
  }

  public function getLastProcessTime()
  {
    return parent::Select("* from GlobalProperties where name='lastProcessTask'");
  }  

  public function getLastPushTaskTZ()
  {
    return $this->clang->convertToTimeZone($this->clang->getLastPushTask());
  }

  public function getSensorsComInfo()
  {
    $module = new BTopSensors($this);
    return array( "nbAvailable" => $module->getNumberOfSensors(),
		  "nbGrab" => $module->getNumberOfSensorsGrab() );
  }

  public function getLastProcessedTimeString()
  {
    $data = $this->getLastProcessTime();
    $timestr = $data[0]["value"];
    return $timestr;
  }

  public function getStatus()
  {
    $sensors = $this->getSensorsComInfo();
    $res = array( array( "id" => "BTopServer",
			 "status" => 1,
			 "msg" => urlencode(_("BlueCountManager running") . " " . $this->getLastPushTaskTZ())),
		  array( "id" => "BTopServer",
			 "status" => $sensors["nbGrab"] == $sensors["nbAvailable"] ? 1 : 2,
			 "msg" => urlencode(_("Sensor(s) communication") . " " . $sensors["nbGrab"] . " / " . $sensors["nbAvailable"])) );
    return $res;
  }
}

?>
