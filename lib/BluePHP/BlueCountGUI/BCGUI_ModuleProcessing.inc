<?php

include_once("BluePHP/Utils/ModuleProcessing.inc");
include_once("BluePHP/BlueCountGUI/Config.inc");

class BCGUI_ModuleProcessing extends ModuleProcessing
{
  protected function __construct($params, $sharedCon = null)
  {
    parent::__construct($params, $sharedCon);
  }

  protected function connect()
  {
    if(!$this->getConParams())
      {	    
	$this->setConParams( array( "type" => SESSION_DBTYPE,
				    "host" => SESSION_DBHOST,
				    "name" => SESSION_DBNAME,
				    "user" => SESSION_DBUSER,
				    "passwd" => SESSION_DBPASSWD ));
      }
    return parent::connect();
  }

  private function generateValuesReplace($params)
  {
    $str = "";
    $sep = "";
    foreach(parent::getModuleStructure() as $k => $v)
      {
	if(!is_numeric($params[$k]))
	  {
	    $str .= $sep . "'" . $params[$k] . "'";
	  }
	else
	  {
	    $str .= $sep . $params[$k];
	  }
	$sep = ",";
      }
    return $str;
  }

  protected function replaceEntry($params)
  {
    parent::QueryNoReturn("replace into " . $this->getName() . " values(" . 
			  $this->generateValuesReplace($params) . ")");
    return $params;
  }
}

?>