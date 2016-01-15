<?php

include_once('ServerRef.inc');

class SensorRef 
{
  public $serverRef;
  public $clientId;
  public $id;
  
  
//   public function __construct($serverRef, $clientId, $id)
//   {
//     $this->serverRef = $serverRef;
//     $this->id = $id;
//     $this->clientId = $clientId;
//   }
  
  public function __construct($serverRef, $clientId = -1, $id = -1)
  {
    $this->serverRef = $serverRef;
    $this->id = $id;
    $this->clientId = $clientId;
  }

  public function setFromName($sensor_name)
  {
    $ref = $this->extractSensorRef($sensor_name);
    $this->id = $ref["id"];
    $this->clientId = $ref["clientId"];
  }
  
  /** Extract from sensor name, the client id and the id (channel id)*/
  private function extractSensorRef($sensor_name)
  {    
    $part = explode('_', $sensor_name);
    $nb = count($part);
    $id = $part[$nb-1];
    $clientid = "";
    for($i = 1; $i < $nb - 1; $i++)
      {
	if($clientid != "") $clientid .= "_";
	$clientid = $part[$i];
      }
    
    return array("name" => $sensor_name, "clientId" => $clientid, "id" => $id);
  } 
  
  public function getSensorName()
  {
    return "sensor_" . $this->clientId . "_" . $this->id;
  }

    
}

?>
