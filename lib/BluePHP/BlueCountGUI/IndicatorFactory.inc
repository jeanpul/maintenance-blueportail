<?php

include_once("IndicatorVisitors.inc");
include_once("IndicatorFlows.inc");
include_once("IndicatorNumbering.inc");
include_once("IndicatorWaitingTime.inc");
include_once("IndicatorFlowDoorComp.inc");
include_once("IndicatorVisLocComp.inc");
include_once("IndicatorNumberingLocComp.inc");
include_once("IndicatorOccupancy.inc");

$indicatorsFactory = array ( "flows" => "IndicatorFlows",
			     "visitors" => "IndicatorVisitors",
			     "numbering" => "IndicatorNumbering",
			     "waitingTime" => "IndicatorWaitingTime",
			     "flowDoorComp" => "IndicatorFlowDoorComp",
			     "visLocComp" => "IndicatorVisLocComp",
			     "numberingLocComp" => "IndicatorNumberingLocComp",
                             "occupancy" => "IndicatorOccupancy"
			     );

function getIndicator($glang, $params)
{
  global $indicatorsFactory;  

  $className = $indicatorsFactory[$params["Analysis"]]; 

  return new $className($glang, $params);
}

?>
