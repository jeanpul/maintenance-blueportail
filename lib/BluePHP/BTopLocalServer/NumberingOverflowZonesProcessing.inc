<?php

include_once("BluePHP/Utils/ModuleProcessingInterface.inc");
include_once("BluePHP/BTopLocalServer/NumberingProcessing.inc");
include_once("BluePHP/BTopLocalServer/NumberingOverflowProcessing.inc");

class NumberingOverflowZonesProcessing implements ModuleProcessingInterface
{
  private $id = null;

  public function __construct($id = null)
  {
    $this->id = $id;
  }

  public function getProcessingId()
  {
    return $this->id;
  }

  public function setProcessingId($id)
  {
    $this->id = $id;
  }

  public function getEntries()
  {
    $m = new NumberingProcessing();
    $pc = $m->getProcessingClass("Numbering_overflow", $this->getProcessingId());
    $entries = array();
    foreach($pc->getProcessingParams() as $k => $v)
      {
	$entries[] = array( "id" => $this->getProcessingId(),
			    "idZone" => $v["id"],
			    "Cmax" => $v["Cmax"],
			    "Nqr" => $v["Nqr"] );
      }
    return $entries;
  }

  public function getEntry($params)
  {
    $m = new NumberingProcessing();
    $pc = $m->getProcessingClass("Numbering_overflow", $params["id"]);
    $entry = array();
    foreach($pc->getProcessingParams() as $k => $v)
      {
	if($v["id"] == $params["idZone"])
	  {
	    $entry = array( "id" => $params["id"],
			    "idZone" => $v["id"],
			    "Cmax" => $v["Cmax"],
			    "Nqr" => $v["Nqr"] );
	    break;
	  }
      }
    return $entry;
  }

  public function newEntry($params)
  {
    return null;
  }

  public function setEntry($params)
  {
    // update the parameter of the specified zone
    // then generate the PHP code for the parameters
    $m = new NumberingProcessing();
    $pc = $m->getProcessingClass("Numbering_overflow", $params["id"]);
    $processingParams = array();    
    foreach($pc->getProcessingParams() as $k => $v)
      {
	if($v["id"] == $params["idZone"])
	  {
	    $v["Cmax"] = $params["Cmax"];
	    $v["Nqr"] = $params["Nqr"];
	  }
	$processingParams[] = $v;
      }
    $strParams = var_export($processingParams, TRUE);

    $userClass = sprintf("Numbering_overflow_%s", $params["id"]); 
    $fileClass = BASEPATH . "/BTopLocalServer/indicators/" . $userClass . ".inc";
    $str = "";
    $str = "<?php\n" . 
      "include_once(\"BluePHP/BTopLocalServer/indicators/Numbering_overflow.inc\");\n" .
      "class Numbering_overflow_" . $params["id"] . " extends Numbering_overflow\n" . 
      "{\n" . 
      "function __construct(\$clang = null) {\n" . 
      "parent::__construct(\$clang);\n" . 
      "\$this->setIndicatorParams($strParams);\n" .
      "}\n}\n" .
      "?>";
    file_put_contents($fileClass, $str);
    return $params;
  }

  public function delEntry($params)
  {
  }

  public function getURLEntry($params)
  {
    return "id=" . $params["id"] . "&idZone=" . $params["idZone"];
  }

}

?>