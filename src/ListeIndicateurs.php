<?php

include_once("Config.inc");

try {

  if(count($argv) < 2)
    {
      echo "Erreur:\tArguments manquants\n" .
	"Usage:\tListeIndicateurs.php ref\n" .
	"\tref\t\tIdentifiant du capteur voir ListeIdentifiantCapteur.php\n";
      exit(1);
    }

  include_once("BluePHP/BluePortail/BluePortailLang.inc");
  
  $bpl = new BluePortailLang(array());

  $ref = $argv[1];
  $capteurs = $bpl->getIPTable();
  $clientId = null;
  for($i = 0; $clientId == null && $i < count($capteurs); ++$i)
    {
      if($capteurs[$i]["bluecountId"] == $ref)
	{
	  $clientId = $capteurs[$i]["clientId"];
	}
    }

  if(!$clientId)
    {
      throw new Exception("Identifiant capteur $ref non trouvé");
    }

  $bpl->setClientId($clientId);

  include_once("BluePHP/BTopLocalServer/FlowCountingProcessing.inc");
  $fcp = new FlowCountingProcessing();

  echo "Indicateur FlowCountingProcessing\n" .
    "---------------------------------\n";
  echo sprintf("%16s%16s%16s%16s\n", "Identifiant", "Nom Porte", "Id Cal", "Nom Cal");
  foreach($fcp->getFlowsFromCounter($ref) as $v)
    {
      echo sprintf("%16s%16s%16s%16s\n", $v["fcpId"], $v["fcpName"], $v["calId"], $v["calName"]);
    }

  include_once("BluePHP/BTopLocalServer/ZoneCounting.inc");
  $zcp = new ZoneCounting();

  echo "\nIndicateur ZoneCountingProcessing\n" .
    "---------------------------------\n";
  echo sprintf("%16s%16s%16s%16s\n", "Identifiant", "Nom Zone", "Id Cal", "Nom Cal");
  foreach($zcp->getZonesFromCounter($ref) as $v)
    {
      if(!in_array($v["zcpId"], $zcpExcluded[$clientId]))
	{
	  echo sprintf("%16s%16s%16s%16s\n", $v["zcpId"], $v["zcpName"], $v["calId"], $v["calName"]);
	}
    }
}
catch(Exception $e)
{
  echo "Exception reçue : ", $e->getMessage(), "\n";
}

?>
