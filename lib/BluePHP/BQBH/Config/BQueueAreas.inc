<?php

include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/BlueSystem/BlueSystem.inc");
include_once('BluePHP/BQBH/Process/Config.inc');
include_once('BluePHP/BQBH/Process/ServerConfig.inc');

class BQueueAreas
{
  public function __construct()
  {
  }

  public function getEntries()
  {
    $res = array();

    $bs = new BlueSystem();
    $ret = $bs->loadLicenseFile(BQBHSERVER_LICENSE_FILE);
    
    if($ret == 0)
      {
	$lfile = $bs->getLicenseFile();
	$maxServer = $lfile->prop['NB'];
	for($i = 0; $i < $maxServer; $i++)
	  {
	    $server = new ServerConfig(BQBHSERVER_PATH, $i);
	    array_push($res, $this->serverRow($server));
	  }
      }
    return $res;
  }

  public function getKeyCond($params)
  {
    return "";
  }

  private function serverRow($server)
  {
    $name = "";
    $map = "";
    $status = "";
    $id = $server->serverRef->id;

    switch($server->getServerStatus())
      {
      case -1:
	$status = "undefined";
	break;
      case 0:
	$status = "unused";
	$name = _("unused");
	break;
      case 1:
	$status = "desactivated";
	$name = $server->getUserName();
	break;
      case 2:
	$status = "activated - "; 
	switch($server->getServerProcessStatus()){
	case 0 :
	  $status .= "ok";
	  break;
	case 1 : 
	  $status .= "pb on some channels";
	  break;		    
	case 2 : 
	  $status .= "pb on all channels";
	  break; 
	}
	$status .= '(PS' . $server->getServerProcessStatus() . ')';
	$name = $server->getUserName();
	break;
      }

    return array( "id" => $id, "name" => $name, "map" => $map, "status" => $status );
  }

}

?>