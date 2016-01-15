<?php

include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/PanelHTML.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/TitleBarConfig.inc");

class TitleBarHTML extends PanelHTML
{
  public function __construct($id, $params)
  {
    parent::__construct($id, $params);
  }

  public function getFormContent()
  {
    $module = new TitleBarConfig();

    $params = array_merge( $module->getDefaultParams(),
			   $this->getPanel() );

    $rows = H();

    $rows->row( array( _("Title"),
		       H()->input()->withValue($params["label"])
		       ->withName("label")->withMaxLength(64)
		       ->withType("text"))
		);
    
    $content = parent::getFormContent()->table($rows);

    return $content;
  }
}

?>