<?php

include_once("BluePHP/BlueCountGUI/Dashboards/WidgetFileConfig.inc");

class WaitingTimeCompHistoryConfig extends WidgetFileConfig
{
  public function __construct()
  {
    parent::__construct(get_class($this), $this->getDefaultParams());
  } 

  public function getDefaultParams()
  {
    return array( "id" => null,
		  "label" => "DEFAULT",
		  "serieColor1" => "#4572A7",
		  "serieName1" => "%nameZ1",
		  "serieName2" => "%nameZ2",
		  "serieColor2" => "#AA4643",
		  "zIndex1" => 1,
		  "zIndex2" => 2,
		  "stacking" => "null",
		  "length" => 5,
		  "numId1" => 0, 
		  "numId2" => 0
		  );		  
  }
}

?>