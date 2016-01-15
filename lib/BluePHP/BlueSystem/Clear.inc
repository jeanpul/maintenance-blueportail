<?php

include_once("BluePHP/Utils/ModuleProcessing.inc");

class Clear extends ModuleProcessing
{
  public function __construct($sharedCon = null)
  {
    parent::__construct(array());
  }

  function getEntry($params)
  {
    return array();
  }

  function setEntry($params)
  {
    $str = "source /etc/BEV/scripts/BlueSystem/BackupRestore.inc\n" . 
      "Clear_loadNeededFunctionForModule \"BlueSystem\"\n" . 
      "BackupRestore_clearSettings \"BlueSystem\"\n";
    exec($str);
    return $params;
  }

};

?>
