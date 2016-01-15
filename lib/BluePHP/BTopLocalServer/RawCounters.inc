<?php

include_once("BluePHP/Utils/ModuleProcessing.inc");
include_once("BluePHP/BTopLocalServer/Config.inc");

class RawCounters extends ModuleProcessing
{
  public function __construct($sharedCon = false)
  {
    parent::__construct( array( "name" => "counting", 
				"keyname" => array( "time", "ref" ),
				"sort" => "time" ), 
			 $sharedCon );
    if(!$sharedCon)
      {
	$this->setConParams( array( "type" => COUNTDBTYPE, 
				    "host" => COUNTDBHOST,
				    "name" => COUNTDBNAME,
				    "user" => COUNTDBUSER,
				    "passwd" => COUNTDBPASSWD )
			     );
      }
  }

  public function getCountsByState()
  {
    return array( "counting" => parent::Select("count(*) as count, state from counting group by state"),
		  "numbering" => parent::Select("count(*) as count, state from numbering group by state") );
  }

  public function getDBSize()
  {
    return filesize(COUNTDBNAME);
  }
}

?>