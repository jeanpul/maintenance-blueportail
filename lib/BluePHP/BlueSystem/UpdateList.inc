<?php

include_once("BluePHP/Utils/ModuleProcessing.inc");

class UpdateList extends ModuleProcessing
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
  }

  public function getUpdateList()
  {
    $res = array();
    if(file_exists("/etc/BEV/UpdateList.txt") and filesize("/etc/BEV/UpdateList.txt") > 0)
      {
	$elts = array();
	exec("cat /etc/BEV/UpdateList.txt", $elts);
	foreach($elts as $e)
	  {
	    $eInfo = explode(" ", $e);
	    if(count($eInfo) == 4)
	      {
		$res[] = array( "date" => trim($eInfo[0]),
				"md5sum" => trim($eInfo[1]),
				"file" => trim($eInfo[3]),
				"ret" => trim($eInfo[2]) );					       
	      }
	  }
      }
    return $res;
  }

};

?>
