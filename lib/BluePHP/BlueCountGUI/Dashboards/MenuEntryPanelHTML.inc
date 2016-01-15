<?php

include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/PanelHTML.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/MenuEntryPanelConfig.inc");

/**
 * \class MenuEntryPanelHTML
 */
class MenuEntryPanelHTML extends PanelHTML
{
  public function __construct($id, $params)
  {
    parent::__construct($id, $params);
  }

  public function getFormContent()
  {
    $module = new MenuEntryPanelConfig();

    $params = array_merge( $module->getDefaultParams(),
			   $this->getPanel() );

    $rows = H();

    $rows->row( array( _("Label"),
		       H()->input()->withValue($params["label"])
		       ->withName("label")->withMaxLength(64)
		       ->withType("text"))
		);

    $rows->row( array( _("Icon"),
		       H()->input()->withValue($params["icon"])
		       ->withName("icon")->withMaxLength(64)
		       ->withType("text"))
		);

    $rows->row( array( _("URL"),
		       H()->input()->withValue($params["href"])
		       ->withName("href")->withMaxLength(64)
		       ->withType("text"))
		);

    $content = parent::getFormContent()->table($rows);

    return $content;
  }

}

?>