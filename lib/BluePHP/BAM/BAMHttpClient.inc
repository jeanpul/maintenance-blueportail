<?php

include_once("BluePHP/Utils/CSVReader.inc");
include_once("BluePHP/BAM/BAM.inc");

class BAMHttpClient extends BAM
{
  protected function msg_print_generic($module, $type, $message)
  {
    $message = urlencode($message);
    file_get_contents("http://" . $this->conParams["host"] . "/BAM/Msg_send.php?" . 
		      "module=$module&type=$type&message=$message");
    //    print_r("http://" . $this->conParams["host"] . "/BAM/Msg_send.php?" . 
    //    		      "module=$module&type=$type&message=$message");
  }

  public function update()
  {
    file_get_contents("http://" . $this->conParams["host"] . "/BAM/Msg_update.php");
  }

  public function Msg_get($clientId, $module)
  {
    $msgs = array();
    // this is csv output so parse it
    $csv = new CSVReader( array( "file" => "http://" . $this->conParams["host"] . "/BAM/Msg_get.php?" .
				 "module=$module&clientId=$clientId",
				 "sep" => ";" ) ); 
    $cols = $csv->getColNames();
    if(count($cols))
      {
	$csvData = $csv->getData();
	for($i = 0; $i < $csv->getNbRows(); ++$i)
	  {
	    $m = array();
	    foreach($cols as $c => $cIdx)
	      {
		$m[$c] = $csvData[$c][$i];
	      }
	    $msgs[$csvData["msgId"][$i]] = $m;
	  }
      }
    return $msgs;
  }

}

?>
