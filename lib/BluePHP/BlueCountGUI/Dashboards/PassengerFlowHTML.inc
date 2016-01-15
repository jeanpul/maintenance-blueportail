<?php

include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/BTopLocalServer/FlowCountingProcessing.inc");
include_once("BluePHP/BTopLocalServer/Doors.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/PanelHTML.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/PassengerFlowConfig.inc");

/**
 * \class PassengerFlowHTML
 */
class PassengerFlowHTML extends PanelHTML
{
  public function __construct($id, $params)
  {
    parent::__construct($id, $params);
  }

  protected function getDefaultParams()
  {
    $module = new PassengerFlowConfig();
    return array_merge( $module->getDefaultParams(),
			$this->getPanel() );
  }

  public function getFormContent()
  {
    $params = $this->getDefaultParams();

    $rows = H();

    $rows->row( array( _("Label"),
		       H()->input()->withValue($params["label"])
		       ->withName("label")->withMaxLength(64)
		       ->withType("text"))
		);

    $module = new FlowCountingProcessing();    
    $fcps = $module->getEntries();

    if(is_null($params["numId"]))
      {
	$fcp = current($fcps);
	$fcp["flow"] = $fcp["flowid"]; // fab: until BlueCountLang exist
	$params["numId"] = $fcp["id"];
      }
    else
      {
	$fcp = $module->getEntry( array( "id" => (int) $params["numId"] ));
      }

    $rows->row( array( _("Indicator => Door/Group"),
		       createSelect( $module->getEntries(),
				     array( "id" => "fcpSelect", 
					    "key" => "id",
					    "name" => "numId",
					    "value" => "id,nameF",
					    "sep" => " => ",
					    "default" => $params["numId"],
					    "extra" => "onChange=\"BTopLocalServer.ModuleChangeDirectionFCP('fcpSelect','flowSelect')\"" 
					    ))
		       )
		);

    $module = new Doors();  
    $door = $module->getOrientedEntry(array( "id" => $fcp["flow"] ));
    $door[0]["field"] = "value0"; $door[1]["field"] = "value1";

    $rows->row( array( _("Direction"),
		       createSelect( $door ,
				     array( "id" => "flowSelect",
					    "name" => "field",
					    "key" => "field",
					    "value" => "nameZ1,nameZ2",
					    "default" => $params["field"],
					    "sep" => " => " )))
		);

    $content = parent::getFormContent()->table($rows);

    return $content;
  }

}

?>