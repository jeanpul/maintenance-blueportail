<?php

/**
 *
 Must parse the following XML document :

 <?xml version="1.0" encoding="UTF-8"?>
 <Sensors>
 <StartTime>2011-01-01 12:00:00</StartTime>
 <EndTime>2011-01-01 12:15:00</EndTime>
 <BlueEyeSensor>
 <Id>1</Id>
 <DeviceId>A1,A2,A3,A4</DeviceId>
 <Value>10,20,30,40</Value>
 </BlueEyeSensor>
 <BlueEyeSensor>
 <Id>2</Id>
 <DeviceId>A5,A6 A7</DeviceId>
 <Value>5,10,7</Value>
 </BlueEyeSensor>
 <BlueEyeSensor>
 <Id>3</Id>
 <DeviceId>A8</DeviceId>
 <Value>6</Value>
 </BlueEyeSensor>
 </Sensors>

 * Create an array of sensors where each elt (BlueEyeSensor) is defined as :
 * ID => the Id value from the XML document
 * DEVICEID => the DeviceId value from the XML document
 * VALUE => the sum of the comma separated values from the Value tag
 * Also retrieve the start and end time for the current sensor array.
 *
 */
class PAXOpsParser {
  private $parser;
  private $sensorList = null;
  private $curTag = null;

  function __construct()
  {
    $this->parser = xml_parser_create();
    xml_set_object($this->parser, $this);
    xml_set_element_handler($this->parser, "startElement", "endElement");
    xml_set_character_data_handler($this->parser, "cdata");
  }
     
  public function parse($data) 
  {
    $this->init_sensor_list();
    xml_parse($this->parser, $data);
  }

  function startElement($parser, $tagName, $attrs)
  {
    $this->curTag = $tagName;
    if($tagName == "BLUEEYESENSOR")
      {
	$this->sensorList["BLUEEYESENSOR"][] = array( "ID" => null,
						      "DEVICEID" => null,
						      "VALUE" => null );
      }
  }

  function endElement($parser, $tagName)
  {
    if($tagName == "VALUE" and count($this->sensorList["BLUEEYESENSOR"]))
      {
	// replace comma separated values by its sum
	$sensorIdx = count($this->sensorList["BLUEEYESENSOR"]) - 1;
	$values = explode(",", $this->sensorList["BLUEEYESENSOR"][$sensorIdx]["VALUE"]);
	$sum = (int) 0;
	foreach($values as $v)
	  {
	    $sum += (int) $v;
	  }
	$this->sensorList["BLUEEYESENSOR"][$sensorIdx]["VALUE"] = $sum;
      }
  }

  function cdata($parser, $cdata)
  {
    if($this->curTag)
      {
	if($this->curTag == "STARTTIME" or $this->curTag == "ENDTIME")
	  {
	    $this->sensorList[$this->curTag] .= trim($cdata);
	  }
	else if($this->curTag == "ID" or $this->curTag == "DEVICEID" or $this->curTag == "VALUE")
	  { // retrieve last added sensor
	    if(count($this->sensorList["BLUEEYESENSOR"]))
	      {
		$sensorIdx = count($this->sensorList["BLUEEYESENSOR"]) - 1;
		$this->sensorList["BLUEEYESENSOR"][$sensorIdx][$this->curTag] .= trim($cdata);
	      }
	  }
      }
  }

  /**
   * Returns the array containing the ID and VALUE for each
   * BlueEyeSensor.
   */
  public function getSensorList()
  {
    return $this->sensorList["BLUEEYESENSOR"];
  }

  /**
   * Returns an array containing the start and end time
   * for the current BlueEyeSensor set.
   */
  public function getTime()
  {
    return array( "STARTTIME" => $this->sensorList["STARTTIME"],
		  "ENDTIME" => $this->sensorList["ENDTIME"] );
  }

  /**
   * Prepare data for parsing a new XML doc
   */
  private function init_sensor_list()
  {
    $this->sensorList = array( "STARTTIME" => null, 
			       "ENDTIME" => null,
			       "BLUEEYESENSOR" => array() );
    $this->curTag = null;
  }
}

?>