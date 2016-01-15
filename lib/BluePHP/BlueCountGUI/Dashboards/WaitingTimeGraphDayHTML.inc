<?php

include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/BTopLocalServer/WaitingTimeProcessing.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/PanelHTML.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/WaitingTimeGraphDayConfig.inc");

class WaitingTimeGraphDayHTML extends PanelHTML
{
  public function __construct($id, $params)
  {
    parent::__construct($id, $params);
  }

  public function getFormContent()
  {
    $module = new WaitingTimeGraphDayConfig();

    $params = array_merge( $module->getDefaultParams(),
			   $this->getPanel() );

    $rows = H();						

    $module = new WaitingTimeProcessing();    
    $rows->row( array( _("Indicator => Zone"),
		       createSelect( $module->getEntries(),
				     array( "key" => "id",
					    "name" => "numId",
					    "value" => "id,nameZ",
					    "sep" => " => ",
					    "default" => $params["numId"],
					    "extra" => false ))
		       )
		);

    $rows->row( array( _("Label"),
		       H()->input()->withValue( $params["label"] )
		       ->withType("text")->withName("label") ) );

    $rows->row( array( _("Graph title"),
		       H()->input()->withValue( $params["title"] )
		       ->withType("text")->withName("title") ) );

    $rows->row( array( _("X Title"),
		       H()->input()->withValue( $params["xtitle"] )
		       ->withType("text")->withName("xtitle") ) );

    $rows->row( array( _("Y Title Numbering"),
		       H()->input()->withValue( $params["ytitleN"] )
		       ->withType("text")->withName("ytitleN") ) );

    $rows->row( array( _("Y Title Wait time"),
		       H()->input()->withValue( $params["ytitleWT"] )
		       ->withType("text")->withName("ytitleWT") ) );

    $rows->row( array( _("Y Title Flow"),
		       H()->input()->withValue( $params["ytitleF"] )
		       ->withType("text")->withName("ytitleF") ) );

    $rows->row( array( _("tick interval (s)"),
		       H()->input()->withValue( $params["tickInterval"] )
		       ->withType("text")->withName("tickInterval") ) );

    $rows->row( array( _("Graph color Estimated wait time"),
		       H()->input()->withValue( $params["serieColorWT"] )
		       ->withType("text")->withName("serieColorWT") ) );

    $rows->row( array( _("Graph color Real wait time"),
		       H()->input()->withValue( $params["serieColorRWT"] )
		       ->withType("text")->withName("serieColorRWT") ) );

    $rows->row( array( _("Graph color Numbering"),
		       H()->input()->withValue( $params["serieColorN"] )
		       ->withType("text")->withName("serieColorN") ) );

    $rows->row( array( _("Graph color Flow"),
		       H()->input()->withValue( $params["serieColorF"] )
		       ->withType("text")->withName("serieColorF") ) );

    $rows->row( array( _("Serie name Estimated wait time"),
		       H()->input()->withValue( $params["serieNameWT"] )
		       ->withType("text")->withName("serieNameWT") ) );

    $rows->row( array( _("Serie name Real wait time"),
		       H()->input()->withValue( $params["serieNameRWT"] )
		       ->withType("text")->withName("serieNameRWT") ) );

    $rows->row( array( _("Serie name Numbering"),
		       H()->input()->withValue( $params["serieNameN"] )
		       ->withType("text")->withName("serieNameN") ) );

    $rows->row( array( _("Serie name flow"),
		       H()->input()->withValue( $params["serieNameF"] )
		       ->withType("text")->withName("serieNameF") ) );

    $rows->row( array( _("Calendar"),
		       createTrueFalseSwitch( array( "name" => "withCalendar",
						  "default" => $params["withCalendar"] ))
		       ));

    $rows->row( array( _("Indicator selection"),
		       createTrueFalseSwitch( array( "name" => "withSelect",
						     "default" => $params["withSelect"] ))
		       ));

    $content = parent::getFormContent()->table($rows);

    return $content;
  }
}

?>