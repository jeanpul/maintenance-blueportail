<?php

include_once("Config.inc");

try {

  if(count($argv) < 2)
    {
      echo "Erreur:\tArguments manquants\n" .
	"Usage:\tSupprimeClient.php nomClient client email serveur\n" .
	"\tclient\t\tIdentifiant unique du client voir ListeClients.php\n";
      exit(1);
    }

  include_once("BluePHP/BluePortail/BluePortailLang.inc");
  
  $bpl = new BluePortailLang(array());
  
  $clientId = $argv[1];

  $clients = $bpl->getClientData(array( "clientId" => $clientId));

  if(is_array($clients) and count($clients))
    {
      $bpl->delVirtualClient($clients[0]);
    }
  else
    {
      throw new Exception("Cannot find client $clientId\n");
    }

}
catch(Exception $e)
{
  echo "Exception reçue : " . $e->getMessage() . "\n";
}

?>