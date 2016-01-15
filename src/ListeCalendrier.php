<?php

include_once("Config.inc");

try {

  if(count($argv) < 2)
    {
      echo "Erreur:\tArguments manquants\n" . 
	"Usage:\tListeCalendrier.php client\n" .
	"\tclient\t\tIdentifiant client voir ListeIdentifiantCapteur.php\n";
      exit(1);
    }

  include_once("BluePHP/BTopLocalServer/Calendar.inc");
  
  $cal = new Calendar();

  echo sprintf("%16s%16s%16s%16s\n", "Identifiant", "Nom", "Plage Début", "Plage Fin");
  foreach($cal->getCalendars() as $v)
    {
      echo sprintf("%16s%16s%16s%16s\n", 
		   $v["calId"], 
		   $v["calName"],
		   strftime($v["testts"], strtotime($v["ts"])),
		   strftime($v["testte"], strtotime($v["te"])));
    }

}
catch(Exception $e)
{
  echo "Exception reçue : ", $e->getMessage(), "\n";
}

?>
