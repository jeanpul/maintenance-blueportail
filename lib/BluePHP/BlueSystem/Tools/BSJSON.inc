<?php

class BSJSON
{
  function __construct()
  {
  }

  public function callFunction($func)
  {
    return call_user_func(array($this, $func));
  }

  public function BlueSystem.checkStatus()
  {
    return '{ "func" : "BlueSystem.checkStatus", "answer": [' . "\n" . 
      '{ "id": "Sensor", "status": "runningfalse", "msg": "Systeme en cours d\'execution (Pas de licence)" },' . "\n" . 
      '{ "id": "Sensor", "status": "runningfalse", "msg": "Pas de capteur defini" },' . "\n" . 
      '{}' . "\n" .
      ']}';
  }
}

?>