<?php

include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/BlueSystem/GUI/BSApplication.inc");
include_once("BluePHP/BTopLocalServer/TimeZone.inc");
include_once("BluePHP/BlueSystem/GUI/BSDialogSubmit.inc");

/**
 * \class TimeZoneHTML
 */
class TimeZoneHTML extends BSDialogSubmit
{
  public function __construct($id, $params)
  {
    parent::__construct($id, array(), array_merge( array( "tz" => "UTC" ), $params));
    parent::setContent("BSDialogSubmit");
  }

  public function render()
  {
    $params = $this->getParams();
    $module = new TimeZone();
    $elts = $module->getEntry(array());
    $params = array_merge($params, $elts[0]);
    
    $gui = H()->p("The BlueCount server use this TimeZone to store the counter values." . 
		  "So changing this will affect the future counters date and time.")->withStyle("width: 500px");
    
    $gui->form(createSelect($module->getTimeZones(),
			    array( "name" => "tz",
				   "default" => $params["tz"] )))->withValue(1)->withId("TimeZoneForm");
    
    $this->setTitle("TimeZone configuration");
    $this->setMsg($gui->str());
    $this->eventSubmit(BS_eventRenderWithForm("BTopLocalServer.TimeZoneHTML", $this->getId(), "TimeZoneForm", "submit"));
    parent::render();
  }

  public function submit()
  {
    $module = new TimeZone();
    $module->setEntry($this->getParams());
  }
}

?>