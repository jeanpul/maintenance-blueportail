<?php

include_once("Config.inc");

function showCountingData($dates, $cpList, $fcp, $excluded = null)
{
  $str = sprintf("%2s%20s%8s%8s%8s%8s%8s%8s\n%'-70s\n", 
		 "id", "start", "v0", "c0", "e0",
		 "v1", "c1", "e1", "-");
  $total = array( "value0" => 0, "nbcumul0" => 0, "nbexpected0" => 0,
		  "value1" => 0, "nbcumul1" => 0, "nbexpected1" => 0 );
  
  for($i = 0; $i < count($dates); $i++)
    {
      for($j = 0; $j < count($cpList); $j++)
	{
	  if(($excluded === null) or !in_array($cpList[$j]["cpId"], $excluded))
	    {
	      $indicator = $fcp->getIndicator($cpList[$j]["cpId"]);
	      $data = $indicator->getValuesDay($cpList[$j]["cpId"],
					       mkTimeFromString($dates[$i]));
	      if(is_array($data)) 
		{
		  foreach($data as $v)
		    {
		      $str .= sprintf("%2s%20s%8d%8d%8d%8d%8d%8d\n",
				      $v["id"], $v["start"], $v["value0"],
				      $v["nbcumul0"], $v["nbexpected0"],
				      $v["value1"], $v["nbcumul1"], $v["nbexpected1"]);
		      foreach(array_keys($total) as $key)
			{
			  $total[$key] += $v[$key];
			}
		    }
		}
	    }
	}
    }
  $str .= sprintf("%'-70s\n%22s%8d%8d%8d%8d%8d%8d\n",
		  "-", "Total = ", 
		  $total["value0"], $total["nbcumul0"], $total["nbexpected0"],
		  $total["value1"], $total["nbcumul1"], $total["nbexpected1"]);
  return $str;
}

try {

  if(count($argv) < 4)
    {
      echo "Erreur:\tArguments manquants\n" .
	"Usage:\tVisualiseDonneesJours.php ref dateDebut dateFin\n" .
	"\tref\t\tIdentifiant du capteur voir ListeIdentifiantCapteur.php\n" .
	"\tdateDebut\tDate du début de la période au format 'YYYYMMDD'\n" .
	"\tdateFin\t\tDate de fin de la période au format 'YYYYMMDD'\n";
      exit(1);
    }

  include_once("BluePHP/BluePortail/BluePortailLang.inc");
  
  $bpl = new BluePortailLang(array());

  $ref = $argv[1];
  $period = array( "start" => $argv[2],
		   "end" => $argv[3] );

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

  echo "Indicateur FlowCountingProcessing\n" .
    "---------------------------------\n";
  include_once("BluePHP/BTopLocalServer/FlowCountingProcessing.inc");
  $fcp = new FlowCountingProcessing();
  $fcpList = $fcp->getFlowsFromCounter($ref);
  
  echo showCountingData($res, $fcpList, $fcp);
  
  echo "\nIndicateur ZoneCountingProcessing\n" .
    "---------------------------------\n";
  include_once("BluePHP/BTopLocalServer/ZoneCounting.inc");
  $zcp = new ZoneCounting();
  $zcpList = $zcp->getZonesFromCounter($ref);
  
  echo showCountingData($res, $zcpList, $zcp, $zcpExcluded[$clientId]);

}
catch(Exception $e)
{
  echo "Exception reçue : ", $e->getMessage(), "\n";
}

?>