<?php

include_once("BluePHP/GUI/Window.inc");
include_once("BluePHP/Utils/ModuleInterface.inc");
include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/Utils/Forms.inc");
include_once("EcoCompteurs.inc");

class EcoCompteursHTML extends ModuleInterface
{
  function EcoCompteursHTML($params)
  {
    parent::setName(_("EcoCompteurs"));
    parent::setModule(new EcoCompteurs());
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
	$winTitle = _("EcoCompteurs configuration");
	$winContent = "EcoCompteurs"; 
      }
    else
      {    
	$winTitle = _("EcoCompteurs configuration");
	$winContent = "EcoCompteursEdit"; 
      }
    parent::setTitle($winTitle);
    parent::setContent($winContent);
    parent::setParams($params);
  }

  protected function getTableInput($params)
  {
    $str = "<table>\n";    
    $str .= createTableRow(array( _("ClientId"), 
				  createInputText($params["clientId"],
					       array( "name" => "clientId",
						      "readonly" => false,
						      "extra" => false ))));

    $str .= createTableRow(array( _("ColCounter0"), 
				  createSelect(array( "E1", "E2", "E3", "E4", "E5", "E6", "E7", "E8" ),
					       array( "value" => false,
						      "name" => "colCounter0",
						      "key" => false,
						      "default" => $params["colCounter0"],
						      "extra" => false ))));

    $str .= createTableRow(array( _("ColCounter1"), 
				  createSelect(array( "S1", "S2", "S3", "S4", "S5", "S6", "S7", "S8" ),
					       array( "value" => false,
						      "name" => "colCounter1",
						      "key" => false,
						      "default" => $params["colCounter1"],
						      "extra" => false ))));

    $str .= createTableRow(array( _("Range"), 
				  createSelect( array( array( "key" => "1", "value" => "1 minute" ),
						       array( "key" => "15", "value" => "15 minutes" ),
						       array( "key" => "60", "value" => "1 hour" ) ),
						array( "value" => "value",
						       "name" => "range",
						       "key" => "key",
						       "default" => $params["range"],
						       "extra" => false ))));

    $str .= createTableRow(array( _("TimeZone"), createSelect(timezone_identifiers_list(),
							      array( "value" => false,
								     "name" => "tz",
								     "key" => false,
								     "default" => $params["tz"],
								     "extra" => false ))));

    $str .= createTableRow(array( _("ftp_server"), createInputText($params["ftp_server"],
								   array( "name" => "ftp_server",
									  "readonly" => false,
									  "extra" => false ))));

    $str .= createTableRow(array( _("ftp_port"), createInputText($params["ftp_port"],
							      array( "name" => "ftp_port",
								     "readonly" => false,
								     "extra" => false ))));

    $str .= createTableRow(array( _("ftp_login"), createInputText($params["ftp_login"],
								  array( "name" => "ftp_login",
									 "readonly" => false,
									 "extra" => false ))));

    $str .= createTableRow(array( _("ftp_passwd"), createInputText($params["ftp_passwd"],
								   array( "name" => "ftp_passwd",
									  "readonly" => false,
									  "extra" => false ))));

    $str .= createTableRow(array( _("ftp_dir"), createInputText($params["ftp_dir"],
								array( "name" => "ftp_dir",
								       "readonly" => false,
								    "extra" => false ))));

    $str .= createTableRow(array(_("bluehttp_server"), createInputText($params["bluehttp_server"],
								       array( "name" => "bluehttp_server",
									      "readonly" => false,
									      "extra" => false ))));

    $str .= createTableRow(array( _("bluehttp_port"), createInputText($params["bluehttp_port"],
								      array( "name" => "bluehttp_port",
									     "readonly" => false,
									     "extra" => false ))));

    $str .= createTableRow(array( _("bluehttp_script"), createInputText($params["bluehttp_script"],
									array( "name" => "bluehttp_script",
									       "readonly" => false,
									       "extra" => false ))));
    
    $str .= createTableRow(array( _("Comment"), createInputTextArea($params["comment"],
								    array( "name" => "comment",
									   "rows" => 4,
									   "cols" => 40,
									   "extra" => false ))));    
    $str .= "</table>\n";
    return $str;
  }

  function getTableList($pagename)
  {
    return parent::getModuleTableList(array("ctitle" => _("Add a new EcoCompteur"),
					    "titles" => array( _("ClientId"), _("ColCounter0"), _("ColCounter1"), 
							       _("Ftp"), _("Range"), _("TimeZone"), _("BlueHTTP") ),
					    "cols" => array( "clientId", "colCounter0", "colCounter1", 
							     "ftp_server", "range", "tz", "bluehttp_server" ) ),
				      $pagename);
  }
  
}


?>
