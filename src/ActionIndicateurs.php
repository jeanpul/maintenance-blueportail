<?php

include_once("Config.inc");

function showCountingData($dates, $period, $cpList, $fcp, $excluded = null)
{
  $str = sprintf("%2s%20s%5s%5s%5s%5s%5s%5s\n%'-52s\n", 
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
	      $data = $indicator->getValues($cpList[$j]["cpId"],
					    mkTimeFromString($dates[$i]),
					    $period["hstart"],
					    $period["hend"]);
	      if(is_array($data)) 
		{
		  foreach($data as $v)
		    {
		      $str .= sprintf("%2s%20s%5d%5d%5d%5d%5d%5d\n",
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
  $str .= sprintf("%'-52s\n%22s%5d%5d%5d%5d%5d%5d\n",
		  "-", "Total = ", 
		  $total["value0"], $total["nbcumul0"], $total["nbexpected0"],
		  $total["value1"], $total["nbcumul1"], $total["nbexpected1"]);
  return $str;
}

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

  if(count($argv) < 7)
    {
      echo "Erreur:\tArguments manquants\n" .
	"Usage:\tActionIndicateurs.php ref action dateDebut dateFin heureDebut heureFin\n" .
	"\tref\t\tIdentifiant du capteur voir ListeIdentifiantCapteur.php\n" .
	"\taction\t\tNom de l'opération utilisée parmis [date,show,delete]\n" .
	"\tdateDebut\t\tDate du début de la période au format 'YYYYMMDD'\n" .
	"\tdateFin\t\tDate de fin de la période au format 'YYYYMMDD'\n" .
	"\theureDebut\t\tHeure de début de la période au format HH:mm\n" .
	"\theureFin\t\tHeure de fin de la période au format HH:mm\n";
      exit(1);
    }

  include_once("BluePHP/BluePortail/BluePortailLang.inc");
  
  $bpl = new BluePortailLang(array());

  $ref = $argv[1];
  $action = $argv[2];
  $period = array( "start" => $argv[3],
		   "end" => $argv[4],
		   "hstart" => $argv[5],
		   "hend" => $argv[6] );

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

  if($action == "date")
    {
      echo sprintf("\n%10s%9s%9s\n%'-28s\n",
		   "Date", "Debut", "Fin", "-");
      for($i = 0; $i < count($res); $i++)
	{
	  echo sprintf("%10s%9s%9s\n", 
		       strftime("%Y-%m-%d", mkTimeFromString($res[$i])),
		       substr($period["hstart"],0,2) . ":" . substr($period["hstart"],2,2) . ":00",
		       substr($period["hend"],0,2) . ":" . substr($period["hend"],2,2) . ":00");
	}
    }
  else if($action == "show")
    {      
      include_once("BluePHP/BTopLocalServer/FlowCountingProcessing.inc");
      $fcp = new FlowCountingProcessing();
      $fcpList = $fcp->getFlowsFromCounter($ref);
      
      echo showCountingData($res, $period, $fcpList, $fcp);

      include_once("BluePHP/BTopLocalServer/ZoneCounting.inc");
      $zcp = new ZoneCounting();
      $zcpList = $zcp->getZonesFromCounter($ref);

      echo showCountingData($res, $period, $zcpList, $zcp, $zcpExcluded[$clientId]);
    }
  else if($action == "delete")
    {
      include_once("BluePHP/BTopLocalServer/FlowCountingProcessing.inc");
      $fcp = new FlowCountingProcessing();
      $fcpList = $fcp->getFlowsFromCounter($ref);
      
      deleteCountingData($res, $period, $fcpList, $fcp);

      include_once("BluePHP/BTopLocalServer/ZoneCounting.inc");
      $zcp = new ZoneCounting();
      $zcpList = $zcp->getZonesFromCounter($ref);

      deleteCountingData($res, $period, $zcpList, $zcp, $zcpExcluded[$clientId]);      
    }

}
catch(Exception $e)
{
  echo "Exception reçue : ", $e->getMessage(), "\n";
}

?>