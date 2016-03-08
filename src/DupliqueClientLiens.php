<?php

include_once("Config.inc");

try {

  if(count($argv) < 3)
    {
      echo "Erreur:\tArguments manquants\n" .
	"Usage:\tDupliqueClientLiens.php clientSource clientDest\n" .
	"\tclientSrc\t\tIdentifiant unique du client voir ListeClients.php\n" .
	"\tclientDest\t\tIdentifiant unique du client voir ListeClients.php\n";
      exit(1);
    }

  include_once("BluePHP/BluePortail/BluePortailLang.inc");
  
  $bpl = new BluePortailLang(array());

  $clientSrc = $argv[1];
  $clientDst = $argv[2];
  
  $bpl->cloneBlueCountClientAssoc(array( "srcClientId" => $clientSrc,
					 "dstClientId" => $clientDst ));
}
catch(Exception $e)
{
  echo "Exception reçue : " . $e->getMessage() . "\n";
}

?>