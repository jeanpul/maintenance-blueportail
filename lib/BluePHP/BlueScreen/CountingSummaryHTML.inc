<?php

require_once("BluePHP/Utils/ModuleInterface.inc");
require_once("BluePHP/Utils/Input.inc");
require_once("configs/CountingSummary.inc");

class CountingSummaryHTML extends ModuleInterface
{
  function CountingSummaryHTML($params)
  {
    parent::setName(_("CountingSummary"));
    parent::setModule(new CountingSummary());
    $this->setWindow($params);
  }

  function setWindow($params)
  {
    $params = array_merge( array( "action" => "list"), 
			   $params );

    $winTitle = "";
    $winContent = "";
    if($params["action"] == "list")
      {
	$winTitle = _("CountingSummary configuration");
	$winContent = "CountingSummary"; 
      }
    else
      {    
	$winTitle = _("CountingSummary configuration");
	$winContent = "CountingSummaryEdit"; 
      }
    parent::setTitle($winTitle);
    parent::setContent($winContent);
    parent::setParams($params);
  }

  function getTableInput($params)
  {
    $str = "<table>\n";    
    $str .= createTableRow(array( _("Id"),
				  createInputText($params["id"],
						  array( "name" => "id",
							 "readonly" => false,
							 "extra" => false ))
				  )
			   );

    $str .= createTableRow(array( _("BTopLocalServer IP"),
				  createInputText($params["server_ip"],
						  array( "name" => "server_ip",
							 "readonly" => false,
							 "extra" => false ))
				  )
			   );

    $str .= createTableRow(array( _("NumberingProcessing"),
				  createInputText($params["NumberingProcessing"],
						  array( "name" => "NumberingProcessing",
							 "readonly" => false,
							 "extra" => false ))
				  )
			   );

    $str .= createTableRow(array( _("FlowProcessing"),
				  createInputText($params["FlowProcessing"],
						  array( "name" => "FlowProcessing",
							 "readonly" => false,
							 "extra" => false ))
				  )
			   );

    $str .= createTableRow(array( _("WaitingTimeProcessing"),
				  createInputText($params["WaitingTimeProcessing"],
						  array( "name" => "WaitingTimeProcessing",
							 "readonly" => false,
							 "extra" => false ))
				  )
			   );


    $str .= createTableRow(array( _("Theme"),
				  createSelect( array( "Default" ),
						array( "key" => false,
						       "name" => "theme",
						       "value" => false,
						       "default" => $params["theme"],
						       "extra" => false ))
				  )
			   );

    $str .= createTableRow(array( _("Message"),
				  createInputTextArea($params["msg"],
						      array( "name" => "msg",
							     "rows" => 10,
							     "cols" => 40,
							     "extra" => false ))));

    $str .= "</table>\n";
    return $str;
  }

  function getTableList($pagename)
  {
    return parent::getModuleTableList(array("ctitle" => _("Create a new Waiting Time display"), 
					    "titles" => array( _("Id"), _("Server"), _("Numbering"), 
							       _("Flow"), _("WaitingTime"), 
							       _("Theme"), _("Message") ),
					    "cols" => array( "id", "server_ip", "NumberingProcessing", 
							     "FlowProcessing", "WaitingTimeProcessing", 
							     "theme", "msg" ) ),
				      $pagename);
  }
  
}


?>