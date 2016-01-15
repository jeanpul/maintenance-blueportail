<?php

include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/BlueSystem/GUI/BSApplication.inc");
include_once("BluePHP/BTopLocalServer/Calendar.inc");
include_once("BluePHP/BlueSystem/GUI/BSDialogSubmit.inc");

/**
 * \class CalendarHTML
 */
class CalendarHTML extends BSDialogSubmit
{
  public function __construct($id, $params)
  {
    $params = array_merge(array( "ts" => "00",
				 "te" => "00",
				 "isOpen" => 1,
				 "cal" => 1,
				 "id" => 0,
				 "pri" => 0,
				 "testts" => "%H%M",
				 "testte" => "%H%M"
				 ),
			  $params);

    parent::__construct($id, array(), $params);
    parent::setContent("BSDialogSubmit");
  }

  public function render()
  {
    $params = $this->getParams();
    $module = new Calendar();
    $params = array_merge( $params, $module->getEntry(array( "id" => $params["id"] )) );

    $gui = H()->h2("Time period of counting processing");
    $gui->p("The BlueCount server store and process the counter only during" . 
	    " the following period. Counting values that are outside this period will be lost.")->withStyle("width: 500px");
   
    $data = $this->hoursRangeData(); 
    $formContent = H()->table( array( H()->row( array( _("From"),
						       createSelect( $data,
								     array( "name" => "ts",
									    "key" => "ts",
									    "value" => "hs",
									    "default" => $params["ts"] )) 
						       )
						),
				      H()->row( array( _("To"),
						       createSelect( $data,
								     array( "name" => "te",
									    "key" => "te",
									    "value" => "he",
									    "default" => $params["te"] )) 
						       )
						)
				      ) 
			       );
    
    foreach(array("cal", "id", "pri", "testts", "testte", "isOpen") as $v)
      {
	$formContent->input()->withType("hidden")->withName($v)->withValue( $params[$v] );
      }
    $formContent->input()->withType("hidden")->withName("step")->withValue(1);
    
    $gui->form( $formContent )->withId("CalendarForm");
    
    $this->setTitle("Calendar configuration");
    $this->setMsg($gui->str());
    $this->eventSubmit(BS_eventRenderWithForm("BTopLocalServer.CalendarHTML", $this->getId(), "CalendarForm", "submit"));
    parent::render();
  }

  public function submit()
  {    
    $module = new Calendar();
    $module->setEntry($this->getParams());
  }

  public function hoursRangeData()
  {
    $res = array(); 
    $hs = 0;
    while($hs <= 23) 
      {
	$hsF = sprintf("%02d", $hs);
	$heF = sprintf("%02d", $hs + 1);
	$res[] = array( "hs" => $hsF,
			"he" => $heF,
			"ts" => "2007-01-01 " . $hsF . ":00:00",
			"te" => "2007-01-01 " . $hsF . ":59:00" 
			);
	++$hs;
      }
    return $res;
  }
}

?>