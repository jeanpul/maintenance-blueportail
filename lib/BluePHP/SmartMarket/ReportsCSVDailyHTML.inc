<?php

require_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/GUI/Window.inc");
include_once("Sites.inc");

class ReportsCSVDailyHTML extends Window
{
  var $params = array( "site_id" => null );

  public function __construct($params)
  {
    parent::__construct("ReportsCSVDaily", "ReportsCSVDaily");
    $this->params = array_merge($this->params, $params);
  }

  function getForm()
  {
    $str = "<table>\n";    
    $module = new Sites();
    $str .= createTableRow(array( _("Site"), 
				  createSelect($module->getEntries(),
					       array( "value" => "name",
						      "name" => "site_id",
						      "key" => "id",
						      "default" => $this->params["site_id"],
						      "extra" => false ))));

    $str .= createTableRow(array( _("Date"),
				  createInputDate(strftime("%Y-%m-%d"), array( "name" => "date",
									       "title" => "cal",
									       "format" => "yyyy-MM-dd" ) )));

    $str .= "</table>\n";
    $str .= createSubmit("Download");
    return createForm($str, array("action" => "GenerateReport.php", "method" => "get"));
  }
}


?>