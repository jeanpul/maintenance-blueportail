<?php

function getEditForm($object, $pageCancel, $pageAction, $params, $dataFunction, $inputFunction)
{
  $params = $dataFunction($params);
  $str  = "<div id=\"formWide\" class=\"Boxes\">\n";
  $str .= "<h2>Edit $object</h2>\n";
  $str .= "<form action=\"$pageAction\">\n";
  $str .= "<input type=\"hidden\" name=\"id\" value=\"" . $params["id"] . "\" />\n";
  $str .= $inputFunction($params);
  $str .= "<div class=\"Bars\" id=\"buttons\">\n";
  $str .= "<ul>\n";
  $str .= "<li><input type=\"submit\" name=\"action\" value=\"Apply\" /></li>\n";
  $str .= "<li><input type=\"submit\" name=\"action\" value=\"Delete\" /></li>\n";
  $str .= "</ul>\n";
  $str .= "</div>\n";
  $str .= "</form>\n";
  $str .= "</div>\n";
  return $str;
}

function getCreateForm($object, $pageCancel, $pageAction, $params, $inputFunction)
{
  $str  = "<div id=\"formWide\" class=\"Boxes\">\n";
  $str .= "<h2>Add a new $object</h2>\n";
  $str .= "<form action=\"$pageAction\">\n";
  $str .= $inputFunction($params);
  $str .= "<div class=\"Bars\" id=\"buttons\">\n";
  $str .= "<ul>\n";
  $str .= "<li><input type=\"submit\" name=\"action\" value=\"Apply\" /></li>\n";
  $str .= "</ul>\n";
  $str .= "</div>\n";
  $str .= "</form>\n";
  $str .= "</div>\n";
  return $str;
}

function processUpdate($object, $pageListName, $params, $processUpdateQuery)
{
  $processUpdateQuery($params);
  $str  = "<div id=\"Message\" class=\"Boxes\">\n";
  $str .= "<h2>$object updated.</h2>\n";
  $str .= "</div>\n";
  return $str;
}

function processInsert($object, $pageListName, $params, $processInsertQuery)
{
  $processInsertQuery($params);
  $str  = "<div id=\"Message\" class=\"Boxes\">\n";
  $str .= "<h2>$object created. Click <a href=\"$pageListName?action=create\">here</a> to create a new $object.</h2>\n";
  $str .= "</div>\n";
  return $str;
}

function processDelete($object, $pageListName, $params, $processDeleteQuery)
{
  $processDeleteQuery($params);
  $str  = "<div id=\"Message\" class=\"Boxes\">\n";
  $str .= "<h2>$object deleted.</h2>\n";
  $str .= "</div>\n";
  return $str;
}

function errorInput($msg)
{
  $str  = "<div id=\"Message\" class=\"Boxes\">\n";
  $str .= "<h2>$msg</h2>\n";
  $str .= "</div>\n";  
  return $str;
}

function getClientInputs($params)
{
  global $plang;
  $str = "<table>\n";
  $readonly = "";
  if(isset($params["clientId"]))
    {
      $readonly = "readonly";
    }
  $str .= "<tr><td>Client Name :</td><td><input type=\"text\" name=\"clientName\" value=\"" . $params["clientName"] . "\" $readonly></td></tr>\n";
  $str .= "<tr><td>ClientId :</td><td><input type=\"text\" name=\"clientId\" value=\"" . $params["clientId"] . "\" $readonly></td></tr>\n";
  $str .= "<tr><td>Email :</td><td><input type=\"text\" name=\"email\" value=\"" . htmlentities($params["email"]) . "\"></td></tr>\n";
  $str .= "<tr><td>BlueCount :</td><td><input type=\"text\" size=\"48\" name=\"server\" value=\"" . htmlentities($params["server"]) . "\" $readonly></td></tr>\n";
  $str .= "</table>\n";
  $str .= "<input type=\"hidden\" name=\"access\" value=\"0\" />\n";

  return $str;
}

function getClientData($params)
{
  global $plang;
  $params["clientId"] = $params["id"];
  $data = $plang->getClientData($params);
  return $data[0];
}

function processClientUpdateQuery($params)
{
  global $plang;
  $plang->processClientUpdate($params);
}

function processClientInsertQuery($params)
{
  global $plang;
  $plang->processClientInsert($params);
}

function processClientDeleteQuery($params)
{
  // use remote BluePortail access
  global $plang;
  $plang->processClientDelete($params);
}

function getSensorInputs($params)
{
  global $plang;
  $str = "<table>\n";
  $readonly = "";
  if(isset($params["ref"]))
    {
      $readonly = "readonly";
    }
  $str .= "<tr><td>Ref :</td><td><input type=\"text\" name=\"ref\" value=\"" . $params["ref"] . "\" $readonly></td></tr>\n";
  $clients = $plang->getClients($params);
  $str .= "<tr><td>Clients :</td><td>";
  $str .= "<select name=\"clients[]\" multiple>\n";

  foreach($clients as $obj)
    {
      $selected = "";

      if(isset($params["clients"]) and in_array($obj["clientId"], $params["clients"]))
	{
	  $selected = "selected";
	}
      $str .= "<option value=\"" . $obj["clientId"] . "\" $selected>" . $obj["clientId"] . "</option>\n";
    }
  $str .= "</select>\n";
  $str .= "</td></tr>\n";
  $str .= "</table>\n";
  return $str;
}

function getSensorData($params)
{
  global $plang;

  $params["ref"] = $params["id"];

  $data = $plang->getBlueCountData($params);
  $obj = $data[0];
  $obj["clients"] = array();
  foreach($data as $v)
    {
      $obj["clients"][] = $v["clientId"];
    }
  $obj["ref"] = $params["ref"];

  return $obj;
}

function processSensorUpdateQuery($params)
{
  global $plang;
  $plang->processBlueCountUpdate($params);
}

function processSensorInsertQuery($params)
{
  global $plang;
  $plang->processBlueCountInsert($params);
}

function processSensorDeleteQuery($params)
{
  global $plang;
  $plang->processBlueCountDelete($params);
}

/***********************************************
 *
 * KEY MANAGEMENT
 *
 ***********************************************/

function getKeyInputs($params)
{
  global $plang;
  $str = "<table>\n";
  $readonly = "";
  if(isset($params["keyId"]))
    {
      $readonly = "readonly";
    }
  $str .= "<tr><td>KeyId :</td><td><input type=\"text\" name=\"keyId\" value=\"" . $params["keyId"] . "\" $readonly></td></tr>\n";
  if(isset($params["keyId"]))
    {
      $str .= "<tr><td>ClientId :</td><td><input type=\"text\" name=\"clientId\" value=\"" . $params["clientId"] . "\" $readonly></td></tr>\n";
      $str .= "<tr><td>StartDate :</td><td><input type=\"text\" name=\"startDate\" value=\"" . $params["startDate"] . "\" $readonly></td></tr>\n";
      $str .= "<tr><td>EndDate :</td><td><input type=\"text\" name=\"endDate\" value=\"" . $params["endDate"] . "\" $readonly></td></tr>\n";
      $str .= "<tr><td>ClientId :</td><td><input type=\"text\" name=\"clientId\" value=\"" . $params["clientId"] . "\" $readonly></td></tr>\n";
    }
  else
    {
      $clients = $plang->getClients($params);
      $str .= "<tr><td>Client :</td><td>";
      $str .= "<select name=\"clientId\">\n";
      
      foreach($clients as $obj)
	{
	  $selected = "";
	  
	  if(isset($params["clientId"]) and $params["clientId"] == $obj["clientId"])
	    {
	      $selected = "selected";
	    }
	  $str .= "<option value=\"" . $obj["clientId"] . "\" $selected>" . $obj["clientId"] . "</option>\n";
	}
      $str .= "</select>\n";
      $str .= "</td></tr>\n";
      $str .= "<tr><td>Start Date YYYY-MM-DD [HH:MM:SS] UTC :</td><td>" . getInputDate("startDate", $params["startDate"]) . "</td></tr>\n";
      $str .= "<tr><td>End Date YYYY-MM-DD [HH:MM:SS] UTC :</td><td>" . getInputDate("endDate", $params["endDate"]) . "</td></tr>\n";
    }

  $str .= "<tr><td>Proprietary Name :</td><td><input type=\"text\" name=\"propName\" value=\"" . $params["propName"] . "\" ></td></tr>\n";
  $str .= "<tr><td>Proprietary Email :</td><td><input type=\"text\" name=\"propEmail\" value=\"" . htmlentities($params["propEmail"]) . "\"></td></tr>\n";
  $str .= "</table>\n";
  return $str;
}

function getInputDate($inputName, $defaultValue = "")
{
  $str = '<script language="JavaScript" ID="js1">' . "\n";
  $str .= 'var cal1 = new CalendarPopup();' . "\n";
  $str .= '</script>' . "\n";
  $str .= '<script language="JavaScript">writeSource("js1");</script>' . "\n";
  $str .= '<input type="text" name="' . $inputName . '" value="' . $defaultValue . '" size="25" />' . "\n";
  $str .= '<a href="#" onClick="cal1.select(document.forms[0].' .
    $inputName . ",'anchor1','yyyy-MM-dd 01:00:00'); " . 
    'return false;" title="Open a calendar window"' . 
    'return false;" name="anchor1" id="anchor1">select</a>';
  return $str;
}

function getKeyData($params)
{
  global $plang;
  $params["keyId"] = $params["id"];
  $data = $plang->getKeyData($params);
  return $data[0];
}

function processKeyUpdateQuery($params)
{
  global $plang;
  $plang->processKeyUpdate($params);
}

function processKeyInsertQuery($params)
{
  global $plang;
  $plang->processKeyInsert($params);
}

function processKeyDeleteQuery($params)
{
  global $plang;
  $plang->processKeyDelete($params);
}

/**
 * \param $params array of parameters
 * 
 */
function processListKeys($params)
{
  $keys = array();
  $dir = KEYPATH . $params["clientId"];
  if (file_exists($dir))
    {      
      //echo "processListKeys : $dir";
      $dirhandle = opendir($dir);

      while(($str = readdir($dirhandle)) != FALSE)
	{	
	  if($str != "." && $str != "..")
	    {
	      $keys[] = $str;
	    }
	}
    }
  return $keys;
}

function processCreateNewKeys($params)
{
  $dir = KEYPATH . $params["clientId"];
  if (!file_exists($dir)) mkdir($dir);

  $date = strftime("%Y%m%d%H%M%S");

  $files = array();
  $files["key"] = "client_" . $date . ".key";
  $files["request"] = "request_" . $date . ".pem";
  $files["signed"] = "signed_" . $date . ".pem";
  $files["certkeyp12"] = "cert_" . $date . ".p12";

  $keyout = $dir . "/" . $files["key"];
  $certificatrequest = $dir . "/" . $files["request"];
  $signed = $dir . "/" . $files["signed"];
  $certkeyp12 = $dir . "/" . $files["certkeyp12"];

  $cmd = "openssl req -new -sha1 -newkey rsa:1024 -nodes -keyout "
    . $keyout
    . " -out "
    . $certificatrequest
    . " -subj /C=fr/ST=RA/L=GRENOBLE/CN=www.blueeyevideo.com/emailAddress=info@blueeyevideo.com/O="
    . $params["clientId"];
  
  exec($cmd);

  $authorityHost="192.168.0.2";

  // signe certificate
  $cmd = "curl -o " . $signed
    . " -F file=\"@" . $certificatrequest 
    . "\" http://" . $authorityHost . "/cgi-bin/signe.cgi";

  exec($cmd);

  // cle et certificat au format PKCS12
  $cmd = "openssl pkcs12 -passout pass: -export -clcerts -in " . $signed . " -inkey " . $keyout . " -out " . $certkeyp12;
  exec($cmd);

  return $files;
}

function processRevokeKeys($params)
{
  $authorityHost="192.168.0.2";
  $cmd = "curl http://$authorityHost/cgi-bin/revoke.cgi?clientBluePortail=" . $params["clientId"];
  return exec($cmd);
}


?>
