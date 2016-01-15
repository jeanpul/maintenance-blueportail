<?php

include_once("BluePHP/Utils/ModuleProcessingInterface.inc");
include_once("BluePHP/BTopLocalServer/Config.inc");
include_once("BluePHP/BTopLocalServer/WaitingTimeProcessing.inc");

class WaitingTimePredProcessing implements ModuleProcessingInterface
{
  public function __construct()
  {
  }

  public function getEntries()
  {
    $module = new WaitingTimeProcessing();
    $data = $module->getEntries();
    $entries = array();
    foreach($data as $k => $v)
      {
	if($v["wt_type"] == "WaitingTime_pred")
	  {
	    $pc = $module->getProcessingClass($v["wt_type"], $v["id"]);
	    $entries[$k] = array_merge( array( "id" => $v["id"], 
					       "nameZ" => $v["nameZ"] ),					
					$pc->getProcessingParams() );
	  }
      }
    return $entries;
  }

  public function getEntry($params)
  {
    $module = new WaitingTimeProcessing();
    $pc = $module->getProcessingClass("WaitingTime_pred", $params["id"]);
    return array_merge( array( "id" => $params["id"] ),
			$pc->getProcessingParams() );
  }

  public function newEntry($params)
  {
    return null;
  }

  public function setEntry($params)
  {
    $userClass = sprintf("WaitingTime_pred_%s", $params["id"]); 
    $fileClass = BASEPATH . "/BTopLocalServer/indicators/" . $userClass . ".inc";
    $str = "";
    $str = "<?php\n" . 
      "include_once(\"BluePHP/BTopLocalServer/indicators/WaitingTime_pred.inc\");\n" .
      "class WaitingTime_pred_" . $params["id"] . " extends WaitingTime_pred\n" . 
      "{\n" . 
      "function __construct(\$clang) {\n" . 
      "parent::__construct(\$clang);\n" . 
      "\$this->setFlowSmooth(" . $params["FlowSmooth"] . ");\n" . 
      "\$this->setMinFlowLevel(" . $params["MinFlowLevel"] . ");\n" . 
      "\$this->setMinWTRate(" . $params["MinWTRate"] . ");\n" . 
      "}\n}\n" .
      "?>";
    file_put_contents($fileClass, $str);
    return $params;
  }

  public function delEntry($params)
  {
    return null;
  }

  public function getURLEntry($params)
  {
    return "id=" . $params["id"];
  }

}

?>