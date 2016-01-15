<?php

include_once("BluePHP/Utils/ModuleProcessing.inc");
include_once("BluePHP/BTopLocalServer/Config.inc");
include_once("BluePHP/BTopLocalServer/BlueCountLang.inc");

class FlowCountingProcessing extends ModuleProcessing
{
  private $clang = false;

  function __construct($sharedCon = null)
  {
    parent::__construct( array( "name" => "FlowCountingProcessing",
				"keyname" => "id",
				"sort" => "id" ),
			 $sharedCon);
    $this->clang = new BlueCountLang();
  }

  function getCalendars()
  {
    return parent::Select("* from Calendars");
  }

  function getFlows()
  {
    return $this->clang->getFlows();
  }

  function getFunctions()
  {
    return parent::Select("* from FlowCountingType");
  }

  function getEntries()
  {
    return $this->clang->getFlowCountingProcessing();
  }

  public function setCalendar($fcpId, $calId)
  {
    parent::QueryNoReturn("update FlowCountingProcessing set calendar=$calId where id=$fcpId");
  }

  function getFlowsFromCounter($ref)
  {
    return parent::Select("FlowCountingProcessing.id as fcpId, Flows.name as fcpName, Calendars.name as calName, Calendars.id as calId, CountersAssoc.sens as sens" . 
			  " from Flows, Calendars, FlowCountingProcessing, CountersAssoc, CountersAvailable where clientId='$ref'" .
			  " and CountersAvailable.id=CountersAssoc.counter and CountersAssoc.flow=Flows.id and" .
			  " Flows.id=FlowCountingProcessing.flow and FlowCountingProcessing.calendar=Calendars.id" . 
			  " order by fcpId");
  }
}

?>
