<?php

include_once("Config.inc");

try {

  include_once("BluePHP/BluePortail/BluePortailLang.inc");

  $bpl = new BluePortailLang(array());

  $clients = $bpl->getClients();

  echo sprintf("%16s%16s%28s%32s\n", "Nom", "Client", "E-Mail", "Serveur");
  foreach($clients as $c)
    {
      echo sprintf("%16s%16s%28s%32s\n", $c["clientName"], $c["clientId"], $c["email"], $c["server"]);
    }
}
catch(Exception $e)
{
  echo "Exception reçue : " . $e->getMessage() . "\n";
}

?>