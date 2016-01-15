<?php

include_once("Config.inc");

try {

  include_once("BluePHP/BluePortail/BluePortailLang.inc");
  
  $bpl = new BluePortailLang(array());

  $capteurs = $bpl->getIPTable(array("extra" => "order by clientId"));
  
  echo sprintf("%22s%26s%16s\n", "Identifiant", "Nom", "Client");
    foreach($capteurs as $v)
    {
      echo sprintf("%22s%26s%16s\n", $v["bluecountId"], $v["name"], $v["clientId"]);
    }
}
catch(Exception $e)
{
  echo "Exception reçue : ", $e->getMessage(), "\n";
}

?>
