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


try {

  if(count($argv) < 4)
    {
      echo "Erreur:\tErreur: Arguments manquants\n" .
          "Usage:\tGenereTachesJours.php ref dateDebut dateFin\n" .
          "\tAjoute les tâches par jour pour les données ZoneCountingProcessing et FlowCountingProcessing pour " .
          "l'intercalle [ dateDebut ; dataFin [\n" .
          "\tref\t\tIdentifiant du capteur voir ListeIdentifiantCapteur.php\n" .
          "\tdateDebut\tDate du début de la période au format 'YYYYMMDD'\n" .
          "\tdateFin\t\tDate de fin de la période au format 'YYYYMMDD'\n";          
      exit(1);
    }

  include_once("BluePHP/BluePortail/BluePortailLang.inc");
  
  $bpl = new BluePortailLang(array());

  $ref = $argv[1];
  $period = array( "start" => $argv[2], "end" => $argv[3] );
    
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
  $fcp = new FlowCountingProcessing();
  $fcpList = $fcp->getFlowsFromCounter($ref);

  addTasksDAY("FlowCounting", $tasks, $res, $fcpList);

  include_once("BluePHP/BTopLocalServer/ZoneCounting.inc");
  $zcp = new ZoneCounting();
  $zcpList = $zcp->getZonesFromCounter($ref);

  addTasksDAY("ZoneCounting", $tasks, $res, $zcpList, $zcpExcluded[$clientId]);
}
catch(Exception $e)
{
  echo "Exception reçue : ", $e->getMessage(), "\n";
}


?>