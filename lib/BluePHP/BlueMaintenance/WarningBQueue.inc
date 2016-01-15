<?php

include_once("BluePHP/BlueMaintenance/BaseMaintenance.inc");
include_once("BluePHP/BlueMaintenance/WarningInsert.inc");

class WarningBQueue extends WarningInsert {

  public function SelectRowWarningHistoric($params, $projectNumber,$useTimezone = false) {
    return $this->SelectRowWarningHistoricGeneric($params, $projectNumber, $useTimezone);
   }
}

?>