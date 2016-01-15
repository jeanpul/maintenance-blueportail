<?php

include_once("Config.inc");

try {

  if(count($argv) < 3)
    {
      echo "Erreur:\tArguments manquants\n" . 
	"Usage:\tChangePlageHoraire.php ref calendrier\n" .
	"\tref\t\tIdentifiant du capteur voir ListeIdentifiantCapteur.php\n" .
	"\tcalendrier\tIdentifiant du calendrier voir ListeCalendrier.php\n";
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

  include_once("BluePHP/BTopLocalServer/Calendar.inc");

  $cal = new Calendar();
  $calendrier = $argv[2];
  $calId = null;
  $cals = $cal->getCalendars();
  for($i = 0; $calId == null && $i < count($cals); ++$i)
    {
      if($cals[$i]["calId"] == $calendrier)
	{
	  $calId = $calendrier;
	}
    }

  if(!$calId)
    {
      throw new Exception("Identifiant calendrier $calendrier non trouvé");
    }

  include_once("BluePHP/BTopLocalServer/FlowCountingProcessing.inc");
  $fcp = new FlowCountingProcessing();
  foreach($fcp->getFlowsFromCounter($ref) as $v)
    {
      $fcp->setCalendar($v["fcpId"], $calendrier);
    }
  
  include_once("BluePHP/BTopLocalServer/ZoneCounting.inc");
  $zcp = new ZoneCounting();
  foreach($zcp->getZonesFromCounter($ref) as $v)
    {
      if(!in_array($v["zcpId"], $zcpExcluded[$clientId]))
	{
	  $zcp->setCalendar($v["zcpId"], $calendrier);
	}
    }
}
catch(Exception $e)
{
  echo "Exception reçue : ", $e->getMessage(), "\n";
}

?>
