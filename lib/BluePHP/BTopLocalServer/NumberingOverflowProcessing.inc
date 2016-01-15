<?php

include_once("BluePHP/Utils/ModuleProcessingInterface.inc");
include_once("BluePHP/BTopLocalServer/NumberingProcessing.inc");

class NumberingOverflowProcessing implements ModuleProcessingInterface
{
  public function __construct()
  {
  }

  public function getEntries()
  {
    $module = new NumberingProcessing();
    $data = $module->getEntries();
    $entries = array();
    foreach($data as $k => $v)
      {
	if($v["numb_type"] == "Numbering_overflow")
	  {
	    $pc = $module->getProcessingClass($v["numb_type"], $v["id"]);
	    $entries[$k] = $v;
	  }
      }
    return $entries;
  }

  public function getEntry($params)
  {
    $module = new NumberingProcessing();
    $pc = $module->getProcessingClass("Numbering_overflow", $params["id"]);
    return array_merge( array( "id" => $params["id"] ),
			$pc->getProcessingParams());
  }

  public function newEntry($params)
  {
    return null;
  }

  public function setEntry($params)
  {
  }

  public function delEntry($params)
  {
  }

  public function getURLEntry($params)
  {
    return "id=" . $params["id"];
  }

}

?>