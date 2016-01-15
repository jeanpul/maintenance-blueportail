<?php

include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/BTopLocalServer/WaitingTimeProcessing.inc"); 
include_once("BluePHP/BlueCountGUI/Dashboards/PanelHTML.inc");

/**
 * \class WaitingTimeHTML
 */
class WaitingTimeHTML extends PanelHTML
{
  public function __construct($id, $params)
  {
    parent::__construct($id, $params);
  }

  public function getFormContent()
  {
    $params = array_merge( array( "label" => "%nameZ",
				  "numId" => null ),
			   $this->getPanel() );

    $rows = H();

    $rows->row( array( _("Label"),
		       H()->input()->withValue($params["label"])
		       ->withName("label")->withMaxLength(64)
		       ->withType("text"))
		);

    $module = new WaitingTimeProcessing();
    
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