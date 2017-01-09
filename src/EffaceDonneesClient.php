<?php

include_once("Config.inc");

function deleteCountingData($dates, $period, $cpList, $fcp, $excluded = null)
{
  for($i = 0; $i < count($dates); $i++)
    {
      for($j = 0; $j < count($cpList); $j++)
          {
              if(($excluded === null) or !in_array($cpList[$j]["cpId"], $excluded))
                  {
                      $indicator = $fcp->getIndicator($cpList[$j]["cpId"]);
                      $indicator->deleteValues($cpList[$j]["cpId"],
                      mkTimeFromString($dates[$i]),
                      $period["hstart"],
                      $period["hend"]);
                  }
          }
    }
}

function getClientIdFromRef($ref)
{
    $elts = explode("_", $ref, 2);
    return $elts[0];
}

try {

  if(count($argv) < 6)
    {
      echo "Erreur:\tArguments manquants\n" .
          "Usage:\tEffaceDonnees.php clientId dateDebut dateFin heureDebut heureFin\n" .
          "\tEfface les données FlowCountingProcessing et ZoneCountingProcessing pour " .
          "l'intervalle [ dateDebut heureDebut ; dateFin heureFin [\n" .
          "\tclientId\t\tIdentifiant du client voir ListeClients.php\n" .
          "\tdateDebut\tDate du début de la période au format 'YYYYMMDD'\n" .
          "\tdateFin\t\tDate de fin de la période au format 'YYYYMMDD'\n" .
          "\theureDebut\tHeure de début de la période au format HHmm\n" .
          "\theureFin\tHeure de fin de la période au format HHmm\n";
      exit(1);
    }

  include_once("BluePHP/BluePortail/BluePortailLang.inc");
  
  $bpl = new BluePortailLang(array());

  $ref = $argv[1];
  $period = array( "start" => $argv[2],
		   "end" => $argv[3],
		   "hstart" => $argv[4],
		   "hend" => $argv[5] );

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

  include_once("BluePHP/Utils/DateOps.inc");
  $res = getTimeSlicesAsStrings($period["start"] . "000000",
				$period["end"] . "000000",
				array( 'second' => 0,
				       'minute' => 0,
				       'hour' => 0,
				       'day' => 1,
				       'month' => 0,
				       'year' => 0 )
				);

  include_once("BluePHP/BTopLocalServer/FlowCountingProcessing.inc");
  include_once("BluePHP/BTopLocalServer/ZoneCounting.inc");
  $fcp = new FlowCountingProcessing();
  $zcp = new ZoneCounting();

  $refs = $bpl->getBlueCountData([ "clientId" => $clientId ]);

  $sensorsClientIds = [];
  
  foreach($refs as $ref) {

      $sensorClientId = getClientIdFromRef($ref["ref"]);
      if(!isset($sensorsClientIds[$sensorClientId])) {
                    
          $fcpList = $fcp->getFlowsFromCounter($sensorClientId);
          deleteCountingData($res, $period, $fcpList, $fcp); 
          
          $zcpList = $zcp->getZonesFromCounter($sensorClientId);
          deleteCountingData($res, $period, $zcpList, $zcp, $zcpExcluded[$clientId]);
      }
  }
  
}
catch(Exception $e)
{
  echo "Exception reçue : ", $e->getMessage(), "\n";
}

?>