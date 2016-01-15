<?php

include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/BTopLocalServer/NumberingProcessing.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/PanelHTML.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/OccupancyConfig.inc");

/**
 * \class OccupancyHTML
 */
class OccupancyHTML extends PanelHTML
{
  public function __construct($id, $params)
  {
    parent::__construct($id, $params);
  }

  public function getFormContent()
  {
    $module = new OccupancyConfig();

    $params = array_merge( $module->getDefaultParams(),
			   $this->getPanel() );

    $rows = H();
    
    $rows->row( array( _("Label"),
		       H()->input()->withValue($params["label"])
		       ->withName("label")->withMaxLength(64)
		       ->withType("text"))
		);
    
    $module = new NumberingProcessing();
    
    $rows->row( array( _("Location/Area"),
		       createSelect( $module->getEntries(),
				     array( "key" => "id",
					    "name" => "numId",
					    "value" => "nameZ",
					    "default" => $params["numId"],
					    "extra" => false ))
		       )
		);

    $content = parent::getFormContent()->table($rows);

    return $content;
  }

}

?>