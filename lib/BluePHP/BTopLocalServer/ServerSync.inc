<?php

include_once("BluePHP/Utils/Input.inc");

class ServerSync
{
  private $SyncFile = "/etc/BEV/sync.rc";

  public function __construct()
  {
  }

  function getValues()
  {
    $res = array( "SYNC_ENABLED" => "false",
		  "SYNC_SERVER" => "" );
    $str = ". " . $this->SyncFile . "\n" .
      'echo SYNC_ENABLED=$SYNC_ENABLED' . "\n" . 
      'echo SYNC_SERVER=$SYNC_SERVER' . "\n";
    exec($str, $output);
    $res = parseSimplePairValues($output);
    return $res;    
  }

  function setValues($params)
  {
    $params = array_merge( array( "SYNC_ENABLED" => "false",
				  "SYNC_SERVER" => "" ),
			   $params );

    $str = "echo SYNC_ENABLED=" . $params["SYNC_ENABLED"] . " > " . $this->SyncFile . "\n" .
      "echo SYNC_SERVER=" . $params["SYNC_SERVER"] . " >> " . $this->SyncFile . "\n";
    exec($str);
    return $params;
  }

  function restore($adress)
  {
    $timeStr = strftime("%Y%m%d000000");
    $str = "source /etc/BEV/scripts/BTopLocalServer/BTopServer.inc\n" . 
      "SYNC_SERVER=" . $adress . "\n" . 
      "BTopServer_Sync_restore $timeStr\n";
    $output = array();
    $res = null;
    exec($str, $output, $res);
    return $res;
  }

};


?>