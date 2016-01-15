<?php

include_once("BluePHP/BlueCountGUI/Dashboards/PassengerFlowHTML.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/FlowHistoryConfig.inc");

/**
 * \class FlowHistoryHTML
 */
class FlowHistoryHTML extends PassengerFlowHTML
{
  public function __construct($id, $params)
  {
    parent::__construct($id, $params);
  }

  protected function getDefaultParams()
  {
    $module = new FlowHistoryConfig();
    return array_merge( $module->getDefaultParams(),
			$this->getPanel() );    
  }
}


?>