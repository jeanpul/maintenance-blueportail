<?php

function checkTimeFormat($tf)
{
  // must be HHMM
  return (strlen($tf) == 4 &&
	  is_numeric($tf) &&
	  (((int) substr($tf,0,2)) >= 0) &&
	  (((int) substr($tf,0,2)) <= 24) &&
	  (((int) substr($tf,2,2)) >= 0) &&
	  (((int) substr($tf,2,2)) <= 59));
}

include_once("Config.inc");

try {

  if(count($argv) < 5)
    {
      echo "Erreur:\tArguments manquants\n" . 
	"Usage:\tAjoutSimpleCalendrier.php client nom début fin\n" .
	"\tclient\t\tIdentifiant client voir ListeIdentifiantCapteur.php\n" .
	"\tnom\t\tNom du calendrier\n" .
	"\tdébut\tHoraire de départ au format HHMM\n" .
	"\tfin\tHoraire de fin au format HHMM\n";
      exit(1);
    }

  include_once("BluePHP/BluePortail/BluePortailLang.inc");

  $bpl = new BluePortailLang(array());

  $clientId = $argv[1];
  $nom = $argv[2];
  $debut = $argv[3];
  $fin = $argv[4];

  $clientIdInfo = $bpl->getClientData(array("clientId" => $clientId));

  if(!count($clientIdInfo))
    {
      throw new Exception("Client $clientId non trouvé");
    }

  $bpl->setClientId($clientId);

  if(!checkTimeFormat($debut))
    {
      throw new Exception("Le format de l'horaire de début est incorrect");
    }

  if(!checkTimeFormat($fin))
    {
      throw new Exception("Le format de l'horaire de fin est incorrect");
    }

  if(strtotime($debut) >= strtotime($fin))
    {
      throw new Exception("L'heure de début doit être inférieure à l'heure de fin");
    }
  
  include_once("BluePHP/BTopLocalServer/Calendar.inc");
  
  $cal = new Calendar();
  $calData = $cal->createCalendar(array( "name" => $nom ));
  $cal->newEntry(array( "cal" => $calData["id"],
			"pri" => 0,
			"testts" => "%H%M",
			"ts" => "2007-01-01 " . substr($debut, 0, 2) . 
			":" . substr($debut, 2, 2) . ":00",
			"testte" => "%H%M",
			"te" => "2007-01-01 " . substr($fin, 0, 2) . 
			":" . substr($fin, 2, 2) . ":00",
			"isOpen" => 1 ));
			
}
catch(Exception $e)
{
  echo "Exception reçue : " . $e->getMessage() . "\n";
}

?>
