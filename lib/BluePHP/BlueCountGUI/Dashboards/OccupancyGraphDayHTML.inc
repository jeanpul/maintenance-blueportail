<?php

include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/BTopLocalServer/NumberingProcessing.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/PanelHTML.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/OccupancyGraphDayConfig.inc");

class OccupancyGraphDayHTML extends PanelHTML
{
  public function __construct($id, $params)
  {
    parent::__construct($id, $params);
  }

  public function getFormContent()
  {
    $module = new OccupancyGraphDayConfig();

    $params = array_merge( $module->getDefaultParams(),
			   $this->getPanel() );

    $rows = H();						

    $module = new NumberingProcessing();    
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

    $rows->row( array( _("Y Title"),
		       H()->input()->withValue( $params["ytitle"] )
		       ->withType("text")->withName("ytitle") ) );

    $rows->row( array( _("tick interval (s)"),
		       H()->input()->withValue( $params["tickInterval"] )
		       ->withType("text")->withName("tickInterval") ) );

    $rows->row( array( _("Graph color"),
		       H()->input()->withValue( $params["serieColor"] )
		       ->withType("text")->withName("serieColor") ) );

    $rows->row( array( _("data name"),
		       H()->input()->withValue( $params["serieName"] )
		       ->withType("text")->withName("serieName") ) );

//    $rows->row( array( _("Date format"),
//		       H()->input()->withValue( $params["dateFmt"] )
//		       ->withType("text")->withName("dateFmt") ) );
    
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