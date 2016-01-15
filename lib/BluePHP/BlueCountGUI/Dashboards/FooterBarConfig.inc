<?php

include_once("BluePHP/BlueCountGUI/Dashboards/WidgetFileConfig.inc");

class FooterBarConfig extends WidgetFileConfig
{
  function __construct($sharedCon = null)
  {
    parent::__construct(get_class($this), $this->getDefaultParams());
  }

  public function getDefaultParams()
  {
    return array( "id" => null, 
		  "leftText" => "",
		  "middleText" => "",
		  "rightText" => "" );
  }
}

?>