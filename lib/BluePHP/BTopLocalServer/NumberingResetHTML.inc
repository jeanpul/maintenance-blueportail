<?php

include_once("BluePHP/Utils/ModuleInterface.inc");
include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/BTopLocalServer/NumberingReset.inc");
include_once("BluePHP/BTopLocalServer/NumberingProcessing.inc");

class NumberingResetHTML extends ModuleInterface
{
  public function __construct($id, $params)
  {
    parent::__construct($id, $params);
    parent::setModule(new NumberingReset());
  }

  function getTableInput($params)
  {
    $module = new NumberingProcessing();
    $data = $module->getOneMinuteValues($params);

    $str = "";

    if(is_array($data) and count($data))
      {
    
	$str = "<p style='width: 400px;'>The system use reset values to compute the next values. So changing this value will" . 
	  " affect all future values of the number of persons.</p>";

	$str .= createInputHidden("id", $data[0]["id"]);
	$str .= getBluePortailInputs();
	$str .= createInputHidden("action", $params["action"]);

	$str .= "<table>\n";

	//	$str .= createTableRow( array( _("Date"),
	//				       createInputText($data[0]["start"],
	//						       array( "name" => "start",
	//							      "id" => "DateReset",
	//							      "maxlength" => 32 ))));
	//	
	//	$str .= createTableRow( array( _("Current value"),
	//				       createInputText($data[0]["value"],
	//						       array( "name" => "curValue",
	//							      "readonly" => true,
	//							      "maxlength" => 12 ))));

	$str .= createTableRow( array(_("New value"),
				      createInputText(0,
						      array( "name" => "value",
							     "maxlength" => 12 ))));
	
	$str .= "</table>\n";

	//$str .= "<script type='text/javascript'>Occupancy_resetDatePicker({'id':'DateReset'});</script>\n";
      }

    return $str;
  }

  function getTableList($pagename = null)
  {
    return null;
  }

}



?>