<?php

include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/PanelHTML.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/FooterBarConfig.inc");

class FooterBarHTML extends PanelHTML
{
  public function __construct($id, $params)
  {
    parent::__construct($id, $params);
  }

  public function getFormContent()
  {
    $module = new FooterBarConfig();
    
    $params = array_merge( $module->getDefaultParams(),
			   $this->getPanel() );

    $rows = H();

    $rows->row( array( _("Left"),
		       createInputTextArea($params["leftText"],
					   array( "name" => "leftText",
						  "rows" => 4,
						  "cols" => 60 )))
		);

    $rows->row( array( _("Middle"),
		       createInputTextArea($params["middleText"],
					   array( "name" => "middleText",
						  "rows" => 4,
						  "cols" => 60 )))
		);

    $rows->row( array( _("Right"),
		       createInputTextArea($params["rightText"],
					   array( "name" => "rightText",
						  "rows" => 4,
						  "cols" => 60 )))
		);

    $content = parent::getFormContent()->table($rows);

    return $content;
  }
}

?>