<?php

include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/PanelHTML.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/CameraViewConfig.inc");

/**
 * \class CameraViewHTML
 */
class CameraViewHTML extends PanelHTML
{
  public function __construct($id, $params)
  {
    parent::__construct($id, $params);
  }

  public function getFormContent()
  {
    $module = new CameraViewConfig();

    $params = array_merge( $module->getDefaultParams(),
			   $this->getPanel() );

    $rows = H();

    $rows->row( array( _("Label"),
		       H()->input()->withValue($params["label"])
		       ->withName("label")->withMaxLength(64)
		       ->withType("text"))
		);

    $rows->row( array( _("URL http://"),
		       H()->input()->withValue($params["URL"])
		       ->withName("URL")->withMaxLength(128)
		       ->withType("text"))
		);

    $rows->row( array( _("login"),
		       H()->input()->withValue($params["login"])
		       ->withName("login")->withMaxLength(32)
		       ->withType("text"))
		);

    $rows->row( array( _("password"),
		       H()->input()->withValue($params["passwd"])
		       ->withName("passwd")->withMaxLength(32)
		       ->withType("text"))
		);

    $rows->row( array( _("Refresh rate"),
		       H()->input()->withValue($params["refreshRate"])
		       ->withName("refreshRate")->withMaxLength(8)
		       ->withType("text"))
		);

    $content = parent::getFormContent()->table($rows);

    return $content;
  }

}

?>
