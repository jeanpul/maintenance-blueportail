<?php

include_once("BluePHP/Utils/ModuleProcessing.inc");
include_once("BluePHP/BTopLocalServer/Config.inc");
include_once("BluePHP/BTopLocalServer/BlueCountLang.inc");

class ProcessTimeProcessing extends ModuleProcessing
{
  private $clang = false;

  function __construct($sharedCon = null)
  {
    parent::setParams( array( "name" => "ZoneProcessTimeProcessing",
			      "keyname" => "id",
			      "sort" => "id" ),
		       $sharedCon);
    $this->clang = new BlueCountLang();
  }

  function getCalendars()
  {
    return parent::Select("* from Calendars");
  }

  function getZones()
  {
    return $this->clang->getZonesWithNumberingProcessing();
  }

  function getFunctions()
  {
    return parent::Select("* from ProcessTimeType");
  }

  function getEntries()
  {
    return $this->clang->getZoneProcessTimeProcessing();
  }
}

?>