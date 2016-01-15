<?php

include_once("BluePHP/Window.inc");
include_once("BluePHP/Input.inc");
include_once("BluePHP/Config.inc");
include_once("BTopLocalServer/BlueCountLang.inc");

class AddSimpleSiteHTML extends Window
{
  function AddSimpleSiteHTML($params)
  {
    parent::Window("Add Simple Site", "AddSimpleSite");
    parent::setParams($params);
  }

  function getForm()
  {
    $str = "<table>\n";
    $str .= createTableRow(array( _("BTopLocalServer"), 
				  createInputText("localhost", array("name" => "BTopLocalServer" ) )));
    $str .= createTableRow(array( _("BluePortail"), 
				  createInputText("CRTBretagne", array("name" => "BluePortail" ) )));
    $str .= createTableRow(array( _("Lieu"), 
				  createInputText("empty", array("name" => "Location" ) )));
    $str .= createTableRow(array( _("ClientID"), 
				  createInputText("", array("name" => "ClientId" ) )));
    $str .= createTableRow(array( _("Sens"), 
				  createSelect(array( 1, 2),
					       array( "value" => false,
						      "name" => "sens",
						      "key" => false ))));
    $str .= createTableRow(array( _("Login"), 
				  createInputText("", array("name" => "login" ) )));
    $str .= createTableRow(array( _("Password"), 
				  createInputText("", array("name" => "password" ) )));
    
    $str .= "</table>\n";

    $str .= createSubmit(_("Submit"));

    return $str;
  }

  function getBTopLocalServerURL()
  {
    return "http://" . $this->getParam("BTopLocalServer") . "/BTopLocalServer/BlueCountLang.php";
  }

  function callBTopLocalServer($fun, $funcParams)
  {
    if($this->getParam("BluePortail"))
      {
	$funcParams = array_merge( array("clientBluePortail" => $this->getParam("BluePortail") ), $funcParams );
      }
    return file_get_contents($this->getBTopLocalServerURL() . "?function=$fun&" . http_build_query($funcParams));
  }

  function process()
  {
    $dbpath = "/home/DATA/";      
    if($this->getParam("BluePortail"))
      {
	$dbpath .=  "BluePortail/clients/" . $this->getParam("BluePortail") . "/";
      }
   
    $db = array( "TYPE" => "SQLITE3" ,
		 "HOST" => "localhost",
		 "NAME" => $dbpath . "/BTopLocalServer/processed.db",
		 "USER" => "blue",
		 "PASSWD" => "blue4eye" );

    echo "<ul>\n";

    
    $clang = new BlueCountLang( false, $db);
    
    $loc = $clang->processLocationInsert( array( "name" => $this->getParam("Location"),
						 "virtual" => 0 ) );
    $locid = false;
    
    if($loc)
      {
	$locid = $loc["id"];
	echo "<li>Location \"" . $this->getParam("Location") . "\" created with id $locid</li>\n";
      }
    else
      {
	return false;
      }

    $door = $clang->processDoorInsert( array( "name" => "Porte_" . $this->getParam("Location"),
					      "idZ1" => 8, // EXTERIEUR
					      "idZ2" => $loc["id"] ) );
    
    if($door)
      {
	echo "<li>Door \"" . "Porte_" . $this->getParam("Location") . "\" created with id " . $door["id"] . "</li>\n";
      }
    else
      { 
	return false;
      }
	
    $btopParams = array( "clientId" => $this->getParam("ClientId"),
			 "channel" => 0,
			 "counter" => 0,
			 "host" => "ND",
			 "descr" => "ND",
			 "door" => $door["id"],
			 "sens" => $this->getParam("sens") );
    $btop0 = $clang->processBTopSensorInsert( $btopParams );

    if($btop0)
      {
	echo "<li>Sensor " . $btopParams["clientId"] . " created</li>\n";
      }
    else
      {
	return false;
      }

    $btopParams["counter"] = 1;
    $btopParams["sens"] = $this->getParam("sens") == "1" ? "2" : "1";
    $btop1 = $clang->processBTopSensorInsert( $btopParams );

    if($btop1)
      {
	echo "<li>Sensor " . $btopParams["clientId"] . " created</li>\n";
      }
    else
      {
	return false;
      }

	
    $res = $clang->getVisitorsIds( array( "Entity" => "location" ));
    $zp = $res[$loc["id"]];
    
    $res = $clang->configDB->query("insert into ZoneCountingType values(" . $zp["idp"] . ", 'ZoneCounting_inoutmean', 'A')");

    if($res)
      {
	echo "<li>Add counting type intoutmean to processing " . $zp["idp"] . "</li>\n";
      }
    else
      {
	return false;
      }
    
    $db["NAME"] = $dbpath . "/BlueCountGUI/session.db";
    $bplang = new Config($db);
    
    $login = $this->getParam("login");
    $passwd = $this->getParam("password");
    $title = $this->getParam("Location");
    
    $res = $bplang->configDB->query("INSERT INTO User VALUES('$login','$passwd','support@blueeyevideo.com',1)");
    $res = $bplang->configDB->query("INSERT INTO ConfigUserAssoc VALUES('$login','SITE_TITLE', '$title')");
    $res = $bplang->configDB->query("INSERT INTO ConfigUserAssoc VALUES('$login','AnalysisSelection','enabled')");
    $res = $bplang->configDB->query("INSERT INTO ConfigUserAssoc VALUES('$login','EntitiesSelection','enabled')");
    $res = $bplang->configDB->query("INSERT INTO ConfigUserAssoc VALUES('$login','Analysis','visitors')");
    $res = $bplang->configDB->query("INSERT INTO ConfigUserAssoc VALUES('$login','Step','hour_minute')");
    $res = $bplang->configDB->query("INSERT INTO ConfigUserAssoc VALUES('$login','Entity','location')");
    $res = $bplang->configDB->query("INSERT INTO ConfigUserAssoc VALUES('$login','id','$locid')");
    $res = $bplang->configDB->query("INSERT INTO ConfigUserAssoc VALUES('$login','CSS','styles/CRTBretagne/crt.css')");

    $res = $bplang->configDB->query("INSERT INTO ZonesNameUserAssoc VALUES('$login','$locid','$title')");
    $res = $bplang->configDB->query("INSERT INTO ZonesEnabledUserAssoc VALUES('$login','$locid','true')");

    $res = $bplang->configDB->query("INSERT INTO IndicatorsEnabledUserAssoc VALUES('$login','visitors','true')");

    if($res)
      {
	echo "<li>BlueCountGUI user configured</li>\n";
      }
    else
      {
	return false;
      }

    echo "</ul>\n";
    return true;
  }

}

?>
