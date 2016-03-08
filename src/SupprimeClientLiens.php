<?php

include_once("Config.inc");

try {

  if(count($argv) < 2)
    {
      echo "Erreur:\tArguments manquants\n" .
	"Usage:\tSupprimeClientLients.php client\n" .
	"\tclient\t\tIdentifiant unique du client voir ListeClients.php\n";
      exit(1);
    }

  include_once("BluePHP/BluePortail/BluePortailLang.inc");
  
  $bpl = new BluePortailLang(array());

  $clientId = $argv[1];
  
  $bpl->deleteBlueCountClientAssoc(array( "clientId" => $clientId ));
}
catch(Exception $e)
{
  echo "Exception reçue : " . $e->getMessage() . "\n";
}

?>