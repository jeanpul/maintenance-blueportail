<?php

include_once("Config.inc");

function addTasksDAY($type, $tasks, $dates, $cpList, $excluded = null)
{
    for($i = 0; $i < count($dates); $i++)
        {
            for($j = 0; $j < count($cpList); $j++)
                {
                    if(($excluded === null) or !in_array($cpList[$j]["cpId"], $excluded))
                        {
                            $tasks->addTaskDay($type, $dates[$i], $cpList[$j]["cpId"]);
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

  if(count($argv) < 4)
    {
      echo "Erreur:\tErreur: Arguments manquants\n" .
          "Usage:\tGenereTachesJoursClient.php clientId dateDebut dateFin\n" .
          "\tAjoute les tâches par jour pour les données ZoneCountingProcessing et FlowCountingProcessing pour " .
          "l'intercalle [ dateDebut ; dataFin [\n" .
          "\tref\t\tIdentifiant du client voir ListeClients.php\n" .
          "\tdateDebut\tDate du début de la période au format 'YYYYMMDD'\n" .
          "\tdateFin\t\tDate de fin de la période au format 'YYYYMMDD'\n";          
      exit(1);
    }

  include_once("BluePHP/BluePortail/BluePortailLang.inc");
  
  $bpl = new BluePortailLang(array());

  $ref = $argv[1];
  $period = array( "start" => $argv[2], "end" => $argv[3] );
    
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

  include_once("BluePHP/BTopLocalServer/Tasks.inc");
  $tasks = new Tasks();
  
  include_once("BluePHP/BTopLocalServer/FlowCountingProcessing.inc");
  include_once("BluePHP/BTopLocalServer/ZoneCounting.inc");
  $fcp = new FlowCountingProcessing();
  $zcp = new ZoneCounting();

  $refs = $bpl->getBlueCountData([ "clientId" => $clientId ]);

  $sensorsClientIds = [];
  
  foreach($refs as $ref)
      {
          $sensorClientId = getClientIdFromRef($ref["ref"]);
          if(!isset($sensorsClientIds[$sensorClientId]))
              {
                  $fcpList = $fcp->getFlowsFromCounter($sensorClientId);
                  addTasksDAY("FlowCounting", $tasks, $res, $fcpList);
                  $zcpList = $zcp->getZonesFromCounter($sensorClientId);
                  addTasksDAY("ZoneCounting", $tasks, $res, $zcpList, $zcpExcluded[$clientId]);
                  $sensorsClientIds[$sensorClientId] = true;
              }
      }

}
catch(Exception $e)
{
  echo "Exception reçue : ", $e->getMessage(), "\n";
}


?>