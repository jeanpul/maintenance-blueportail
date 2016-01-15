<?php

include_once("BluePHP/BlueCountGUI/Dashboards/WidgetFileConfig.inc");

class WaitingTimeCompDayConfig extends WidgetFileConfig
{
  public function __construct()
  {
    parent::__construct(get_class($this), $this->getDefaultParams());
  } 

  public function getDefaultParams()
  {
    return array( "id" => null,
		  "label" => "DEFAULT",
		  "title" => "Wait time comparison for %nameZ1 and %nameZ2",
		  "xtitle" => _("Values by minute"),
		  "ytitle" => _("Wait Time (minutes)"),
		  "serieColor1" => "#4572A7",
		  "serieColor2" => "#AA4643",
		  "serieName1" => "%nameZ1",
		  "serieName2" => "%nameZ2",
		  "zIndex1" => 1,
		  "zIndex2" => 2,
		  "stacking" => "null",
		  "tickInterval" => 60,
		  "numId1" => 0, 
		  "numId2" => 0,
		  "withCalendar" => 0 );		  
  }
}

?>