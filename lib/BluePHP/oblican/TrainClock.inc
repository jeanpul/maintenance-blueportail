<?php

include_once("BluePHP/Utils/ModuleProcessing.inc");
include_once("BluePHP/oblican/Config.inc");

class TrainClock extends ModuleProcessing
{
  public function __construct($sharedCon = null)
  {
    parent::__construct(array( "name" => "TRAIN_CLOCK",
			       "keyname" => array( "who", "which", "why" ),
			       "sort" => "whenEvent desc" ), 
			$sharedCon);
  }
  
  protected function connect()
  {
    if(!$this->getConParams())
      {	    
	$this->setConParams( array( "type" => SESSION_DBTYPE,
				    "host" => SESSION_DBHOST,
				    "name" => SESSION_DBNAME,
				    "user" => SESSION_DBUSER,
				    "passwd" => SESSION_DBPASSWD ));
      }
    return parent::connect();
  }

  public function store($sstruct)
  {
    $data = array( "who" => $sstruct["who"],
		   "which" => null,
		   "why" => null,
		   "how" => null,
		   "whenEvent" => strftime("%Y-%m-%d %H:%M:%S"),
		   "whereLocation" => $sstruct["whereLocation"] );
    // try to fill missing attributes
    foreach($sstruct["elts"] as $k => $v)
      {
	if(isset($v["keyword"]))
	  {
	    $data["why"] = $v["keyword"];
	    $data["how"] = $v["value"];
	  }
	else
	  {
	    $data["which"] = $v["value"];
	  }
      }

    // now check if this information was already filled in
    // by the same agent
    $dataPrev = $this->findSameEntry($data);   

    // if not then create a new entry
    // if so update the information
    if(is_null($dataPrev))
      {
	$this->newEntry($data);
      }
    else
      {
	$this->replaceModuleEntry($dataPrev, $data);
      }
    return $data;
  }

  protected function findSameEntry($data)
  {
    $ret = null;
    $res = $this->getEntry($data);
    if(is_array($res))
      {		
	$date1 = new DateTime($res["whenEvent"]);
	$diff = $date1->diff(new DateTime($data["whenEvent"]));
	if($diff->d < 1)
	  {
	    $ret = $res;
	  }
      }
    return $ret;
  }

}

?>