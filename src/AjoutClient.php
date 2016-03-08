<?php

include_once("Config.inc");

try {

  if(count($argv) < 5)
    {
      echo "Erreur:\tArguments manquants\n" .
	"Usage:\tAjoutClient.php nomClient client email serveur\n" .
	"\tnomClient\tNom du client voir ListeClients.php\n" .
	"\tclient\t\tIdentifiant unique du client void ListeClients.php\n" .
	"\temail\t\tAddresse e-mail du client\n" . 
	"\tserveur\t\tAddresse du server ceCountLocalServer (http://localhost/)\n";
      exit(1);
    }

  include_once("BluePHP/BluePortail/BluePortailLang.inc");
  
  $bpl = new BluePortailLang(array());
  
  $bpl->addVirtualClient(array( "clientName" => $argv[1],
				"clientId" => $argv[2],
				"email" => $argv[3],
				"server" => $argv[4],
				"access" => 0 ));

}
catch(Exception $e)
{
  echo "Exception reçue : " . $e->getMessage() . "\n";
}

?>