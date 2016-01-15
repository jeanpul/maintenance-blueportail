<?php

include_once("BluePHP/BTopLocalServer/Config.inc");
include_once("BluePHP/Utils/ModuleProcessing.inc");

class IndicatorExport extends ModuleProcessing
{
  public function __construct($sharedCon = null)
  {
    parent::__construct(array( "name" => "Export",
			       "keyname" => array("id","indicator","module"),
			       "sort" => "id" ),
			$sharedCon);
  }

  public function setEntry($params)
  {
    $params = array_merge( array( "doExport" => 0 ),
			   $params );
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
    parent::QueryNoReturn("replace into Export values(" . $str . ")");
    return $params;  
  }

  public function newEntry($params)
  {
    $params = array_merge( array( "doExport" => 0 ),
			   $params );
    return parent::newEntry($params);
  }

  public function getIndicatorsName()
  {
    return array( "FlowCountingProcessing", "ZoneCountingProcessing", "ZoneNumberingProcessing",
		  "ZoneWaitingTimeProcessing" );
  }

  public function getModulesName()
  {
    return array( "getOneMinuteValues", "GenerateReport" );
  }

  public function getProcessingIds($params)
  {
    return parent::Select("* from " . $params["indicator"] . " order by id asc");
  }
}

?>