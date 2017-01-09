<?php

include_once("Config.inc");

function getClientIdFromRef($ref)
{
    $elts = explode("_", $ref, 2);
    return $elts[0];
}

try {

  if(count($argv) < 2)
    {
      echo "Erreur:\tArguments manquants\n" .
	"Usage:\tListeIndicateurs.php clientId\n" .
	"\tref\t\tIdentifiant du client voir ListeClients.php\n";
      exit(1);
    }

  include_once("BluePHP/BluePortail/BluePortailLang.inc");
  
  $bpl = new BluePortailLang(array());

  $ref = $argv[1];

  $clients = $bpl->getClients();
  $clientId = null;
  
  for($i = 0; $clientId == null && $i < count($clients); ++$i)
    {
        if($clients[$i]["clientId"] == $ref)
            {
                $clientId = $ref;
            }
    }

  if(!$clientId)
    {
      throw new Exception("Identifiant client $ref non trouvé");
    }
  
  $bpl->setClientId($clientId);

  include_once("BluePHP/BTopLocalServer/FlowCountingProcessing.inc");
  include_once("BluePHP/BTopLocalServer/ZoneCounting.inc");
  $fcp = new FlowCountingProcessing();
  $zcp = new ZoneCounting();
  
  echo "Indicateur FlowCountingProcessing\n" .
    "---------------------------------\n";
  echo sprintf("%16s%16s%16s%16s%16s\n", "Ref", "Identifiant", "Nom Porte", "Id Cal", "Nom Cal");
  
  $refs = $bpl->getBlueCountData([ "clientId" => $clientId ]);

  $sensorsClientIds = [];

  foreach($refs as $ref) {
      $sensorClientId = getClientIdFromRef($ref["ref"]);
      if(!isset($sensorsClientIds[$sensorClientId])) {
          foreach($fcp->getFlowsFromCounter($sensorClientId) as $v) {
              echo sprintf("%16s%16s%16s%16s%16s\n", $sensorClientId, $v["fcpId"], $v["fcpName"], $v["calId"], $v["calName"]);
          }
      }
  }

  echo "\nIndicateur ZoneCountingProcessing\n" .
    "---------------------------------\n";
  echo sprintf("%16s%16s%16s%16s%16s\n", "Ref", "Identifiant", "Nom Zone", "Id Cal", "Nom Cal");

  $sensorsClientIds = [];

  foreach($refs as $ref) {
      $sensorClientId = getClientIdFromRef($ref["ref"]);
      if(!isset($sensorsClientIds[$sensorClientId])) {
          foreach($zcp->getZonesFromCounter($sensorClientId) as $v) {
              if(!in_array($v["zcpId"], $zcpExcluded[$clientId])) {
                  echo sprintf("%16s%16s%16s%16s%16s\n", $sensorClientId, $v["zcpId"], $v["zcpName"], $v["calId"], $v["calName"]);
              }
          }
      }
  }
  
}
catch(Exception $e)
{
  echo "Exception reçue : ", $e->getMessage(), "\n";
}

?>
