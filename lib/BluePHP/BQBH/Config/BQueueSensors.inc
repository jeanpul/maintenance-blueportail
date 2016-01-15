<?php

include_once("BluePHP/Utils/Input.inc");

class BQueueSensors
{
  public function __construct()
  {
  }

  public function getEntries()
  {
    $BQBH_CHANNEL_SNAP_DIR="/home/DATA/statussnap/";

    /**
     * This will return the channels (sensors) list with their
     * device information (url for ip sensors) in the form
     * <id channel>=<device info>
     */
    $str = "source /etc/BEV/scripts/Sensor/Config.inc\n" .
      "source /etc/BEV/scripts/BQBH/BQBH.inc\n" .
      'for i in $($BQBH_LISTCHANNEL_CMD) ; do' . "\n" . 
      'echo $i=$($BQBH_EXTRACTCONFIG_CMD $i device)' . "\n" .
      'done';
      
    $output = array();
    exec($str, $output);
    $output = parseSimplePairValues($output);
    $res = array();
    foreach($output as $id => $device)
      {
	array_push($res, array( "id" => $id,
				"view" => "/statussnap/$id.jpg", 
				"device" => $device,
				"status" => "tata" ));
      }
    return $res;
  }

  public function getKeyCond($params)
  {
    return "";
  }
}

?>