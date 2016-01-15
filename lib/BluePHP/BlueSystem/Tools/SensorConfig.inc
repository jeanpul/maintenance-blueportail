<?php

class SensorConfig
{
  private $params = array( "CONFIG_MAX_CHANNEL" => 25,
			   "CONFIG_FILE" => "/etc/BEV/modules/Sensor/Config.rc",
			   "CONFIG_FUNC" => "/etc/BEV/modules/Sensor/ConfigFunctions.inc" );

  private $sensors = array();

  public function getValues()
  {
    if(file_exists($this->params["CONFIG_FUNC"]))
      {
	include_once($this->params["CONFIG_FUNC"]);
      }
  }

  public function haveAllStatusOK()
  {
    $statusOK = true;
    for($it = new ArrayIterator($this->sensors); $statusOK and $it->valid(); $it->next())
      {
	$statusOK = $statusOK and $it->current()->getStatus();
      }
    return $statusOK;
  }

  public function registerSensor($s)
  {
    // todo : check that the sensor does not already exist
    $this->sensors[] = $s;
  }

  public function registerChannelType()
  {
  }
}

?>