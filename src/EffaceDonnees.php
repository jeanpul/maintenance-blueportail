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

try {

  if(count($argv) < 6)
    {
      echo "Erreur:\tArguments manquants\n" .
	"Usage:\tEffaceDonnees.php ref dateDebut dateFin heureDebut heureFin\n" .
	"\tref\t\tIdentifiant du capteur voir ListeIdentifiantCapteur.php\n" .
	"\tdateDebut\tDate du début de la période au format 'YYYYMMDD'\n" .
	"\tdateFin\t\tDate de fin de la période au format 'YYYYMMDD'\n" .
	"\theureDebut\tHeure de début de la période au format HH:mm\n" .
	"\theureFin\tHeure de fin de la période au format HH:mm\n";
      exit(1);
    }

  include_once("BluePHP/BluePortail/BluePortailLang.inc");
  
  $bpl = new BluePortailLang(array());

  $ref = $argv[1];
  $period = array( "start" => $argv[2],
		   "end" => $argv[3],
		   "hstart" => $argv[4],
		   "hend" => $argv[5] );

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

  include_once("BluePHP/BTopLocalServer/FlowCountingProcessing.inc");
  $fcp = new FlowCountingProcessing();
  $fcpList = $fcp->getFlowsFromCounter($ref);
  
  deleteCountingData($res, $period, $fcpList, $fcp);
  
  include_once("BluePHP/BTopLocalServer/ZoneCounting.inc");
  $zcp = new ZoneCounting();
  $zcpList = $zcp->getZonesFromCounter($ref);
  
  deleteCountingData($res, $period, $zcpList, $zcp, $zcpExcluded[$clientId]);      
}
catch(Exception $e)
{
  echo "Exception reçue : ", $e->getMessage(), "\n";
}

?>