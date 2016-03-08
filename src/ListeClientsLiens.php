<?php

include_once("Config.inc");

try {

  include_once("BluePHP/BluePortail/BluePortailLang.inc");
  
  $bpl = new BluePortailLang(array());
  
  $links = $bpl->getLinks();
  
  $refs = array();

  foreach($links as $l)
    {
      $elts = explode("_", $l["bluecountId"]);
      $l["ref"] = $elts[0];
      if(!isset($refs[$elts[0]]))
	{
	  $refs[$elts[0]] = array( $l["clientId"] => $l );
	}
      else if(!isset($refs[$elts[0]][$l["clientId"]]))
	{
	  $refs[$elts[0]][$l["clientId"]] = $l;
	}
    }

  echo sprintf("%32s%16s%32s%12s\n", "Identifiant", "Client", "Serveur", "Activé");
  foreach($refs as $ref)
    {
      foreach($ref as $r)
	{
	  echo sprintf("%32s%16s%32s%12s\n", $r["ref"], $r["clientId"],
		       $r["server"], $r["isActivated"]);
	}
    }
}
catch(Exception $e)
{
  echo "Exception reçue : " . $e->getMessage() . "\n";
}

?>