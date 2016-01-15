<?php

include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/PanelHTML.inc");
include_once("BluePHP/BTopLocalServer/WaitingTimeProcessing.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/WaitingTimeCompDayConfig.inc");

class WaitingTimeCompDayHTML extends PanelHTML
{
  public function __construct($id, $params)
  {
    parent::__construct($id, $params);
  }

  public function getFormContent()
  {
    $module = new WaitingTimeCompDayConfig();

    $params = array_merge( $module->getDefaultParams(),
			   $this->getPanel() );

    $rows = H();

    $module = new WaitingTimeProcessing();    
    $rows->row( array( _("First Indicator"),
		       createSelect( $module->getEntries(),
				     array( "key" => "id",
					    "name" => "numId1",
					    "value" => "id,nameZ",
					    "sep" => " / ",
					    "default" => $params["numId1"],
					    "extra" => false ))
		       )
		);

    $rows->row( array( _("Second Indicator"),
		       createSelect( $module->getEntries(),
				     array( "key" => "id",
					    "name" => "numId2",
					    "value" => "id,nameZ",
					    "sep" => " / ",
					    "default" => $params["numId2"],
					    "extra" => false ))
		       )
		);

    $rows->row( array( _("Calendar"),
		       createTrueFalseSwitch( array( "name" => "withCalendar",
						  "default" => $params["withCalendar"] ))
		       ));

    $rows->row( array( _("Label"),
		       H()->input()->withValue( $params["label"] )
		       ->withType("text")->withName("label") ) );

    $rows->row( array( _("Graph title"),
		       H()->input()->withValue( $params["title"] )
		       ->withType("text")->withName("title") ) );

    $rows->row( array( _("X Title"),
		       H()->input()->withValue( $params["xtitle"] )
		       ->withType("text")->withName("xtitle") ) );

    $rows->row( array( _("Y Title"),
		       H()->input()->withValue( $params["ytitle"] )
		       ->withType("text")->withName("ytitle") ) );

    $rows->row( array( _("First Serie name"),
		       H()->input()->withValue( $params["serieName1"] )
		       ->withType("text")->withName("serieName1") ) );

    $rows->row( array( _("First Serie color"),
		       H()->input()->withValue( $params["serieColor1"] )
		       ->withType("text")->withName("serieColor1") ) );

    $rows->row( array( _("First Serie on top"),
		       createSelect( array( array( "label" => "true", "idx" => 3 ), 
					    array( "label" => "false", "idx" => 1 ) ),
				     array( "key" => "idx",
					    "name" => "zIndex1",
					    "value" => "label",
					    "default" => $params["zIndex1"],
					    "extra" => false ))
		       )
		);

    $rows->row( array( _("Second Serie name"),
		       H()->input()->withValue( $params["serieName2"] )
		       ->withType("text")->withName("serieName2") ) );

    $rows->row( array( _("Second Serie color"),
		       H()->input()->withValue( $params["serieColor2"] )
		       ->withType("text")->withName("serieColor2") ) );

    $rows->row( array( _("Stacking mode"),
		       createSelect( array( "null", "normal", "percent" ),
				     array( "key" => null,
					    "name" => "stacking",
					    "value" => null,
					    "default" => $params["stacking"],
					    "extra" => false ))
		       )
		);

    $content = parent::getFormContent()->table($rows);

    return $content;
  }
}

?>