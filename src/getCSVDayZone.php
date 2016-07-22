<?php

include_once("Config.inc");

function showCountingData($dates, $cpList, $fcp, $idOut, $excluded = null) {
    $str = "";
    for($i = 0; $i < count($dates); $i++) {
        for($j = 0; $j < count($cpList); $j++) {
            if(($excluded === null) or !in_array($cpList[$j]["cpId"], $excluded)) {
                $time = mktimeFromString($dates[$i]);
                $indicator = $fcp->getIndicator($cpList[$j]["cpId"]);
                $data = $indicator->getValuesDay($cpList[$j]["cpId"],
                $time);
                if(is_array($data)) {
                    foreach($data as $v) {
                        $in = "value0"; $out = "value1";                        
                        if( (($cpList[$j]["sens"] == 1) && ($cpList[$j]["zone2"] == $idOut)) ||
                        (($cpList[$j]["sens"] == 2) && ($cpList[$j]["zone1"] == $idOut))) {
                            $in = "value1"; $out = "value0";
                        } 
                        $str .= $cpList[$j]["cpName"] . ";" . strftime("%d/%m/%Y", $time) . ";" . $v[$in] . ";" . $v[$out] . "\n";
                    }
                }
            }
        }
    }
    return $str;
}

try {

  if(count($argv) < 3) {
      echo "Erreur:\tArguments manquants\n" .
          "Usage:\tgetCSVDay.php dateDebut dateFin\n" .
          "\tdateDebut\tDate du début de la période au format 'YYYYMMDD'\n" .
          "\tdateFin\t\tDate de fin de la période au format 'YYYYMMDD'\n";
      exit(1);
  }

  include_once("BluePHP/BluePortail/BluePortailLang.inc");
  include_once("BluePHP/Utils/DateOps.inc");
  
  $bpl = new BluePortailLang(array());

  $refs = $bpl->getIPTable();

  $clientId = "CRTBretagne";

  $bpl->setClientId($clientId);

  include_once("BluePHP/BTopLocalServer/ZoneCounting.inc");
  
  $period = array(
      "start" => $argv[1],
      "end" => $argv[2]
  );

  $res = getTimeSlicesAsStrings(
      $period["start"] . "000000",
      $period["end"] . "000000",
      array(
          'second' => 0,
          'minute' => 0,
          'hour' => 0,
          'day' => 1,
          'month' => 0,
          'year' => 0
      )
  );

  echo "Porte ; Date ; Entrees ; Sorties\n";
  $cp = new ZoneCounting();
  $cpList = $cp->getAllZones();
  echo showCountingData($res, $cpList, $cp, 8, $zcpExcluded[$clientId]);
}

catch(Exception $e) {
    echo "Exception reçue : ", $e->getMessage(), "\n";
}

?>