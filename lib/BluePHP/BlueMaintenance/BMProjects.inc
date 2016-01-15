<?php

include_once("BluePHP/BlueProjectPHP/Projects.inc");
include_once("BluePHP/BlueMaintenance/Config.inc");
include_once("BluePHP/BlueMaintenance/BQueueMaintenance.inc");
include_once("BluePHP/BlueMaintenance/BCMMaintenance.inc");
include_once("BluePHP/BlueMaintenance/BTOPMaintenance.inc");

class BMProjects
{
  private $configPath = null;
  
  public function __construct($params = null) {
    $this->configPath = BLUEMAINTENANCEROOT . "Config";
  }
  
  public function getConfigs()
  {
    $module = new Projects();
    $res = array();
    if($handle = opendir($this->configPath)) 
      {
	while(false != ($entry = readdir($handle))) 
	  {
	    $fullPath = $this->configPath . "/" . $entry;
	    if(is_dir($fullPath) and is_dir($fullPath . "/Projects"))
	      {
		$newElt = array( "name" => $entry,
				 "projects" => array() );
		if($handleProjects = opendir($fullPath . "/Projects"))
		  {
		    while(false != ($entryP = readdir($handleProjects)))
		      {
			$fullPathP = $fullPath . "/Projects/" . $entryP;
			if(is_dir($fullPathP) and file_exists($fullPathP . "/conexion.txt"))
			  {
			    $project = $module->getEntry(array( "project_id" => $entryP ));
			    $newElt["projects"][$entryP] = array( "id" => $entryP,
							   "name" => $project["project_name"] );
			  }
		      }
		  }
		$res[$entry] = $newElt;
	      }
	  }
      }
    return $res;
  }

  public function getConfigStatusOverview($config)
  {
    $bqueue = new BQueueMaintenance();
    $btop = new BTopMaintenance();
    $bcm = new BCMMaintenance();

    $module = new Projects();

    $res = array();
    $projects = $this->getConfigs();

    if(isset($projects[$config]) and 
       count($projects[$config]["projects"]))
      {
	foreach($projects[$config]["projects"] as $k => $v)
	  {
	    $sp = null;
	    $sp = $this->sumOverviewFromProject($sp, 
						$bqueue->getLastEntriesNumeroP($k)
						);
	    $sp = $this->sumOverviewFromProject($sp, 
						$btop->getLastEntriesNumeroP($k)
						);
	    $sp = $this->sumOverviewFromProject($sp, 
						$bcm->getLastEntriesNumeroP($k)
						);		
	    if(!is_null($sp))
	      {
		$sp["project_id"] = $k;
		$entry = $module->getEntry( array( "project_id" => $k ) );
		$sp["project_name"] = $entry["project_name"];
		$sp["nbProject"] += 1;
		$res[$k] = $sp;
	      }
	  }
      }
    return $res;
  }

  public function getStatusOverview()
  {
    $bqueue = new BQueueMaintenance();
    $btop = new BTopMaintenance();
    $bcm = new BCMMaintenance();

    $res = array();
    $projects = $this->getConfigs();
    foreach($projects as $k => $v)
      {
	if(count($v["projects"]))
	  {
	    $sp = null;
	    foreach($v["projects"] as $l => $w)
	      {
		$sp = $this->sumOverviewFromProject($sp, 
						    $bqueue->getLastEntriesNumeroP($l)
						    );
		$sp = $this->sumOverviewFromProject($sp, 
						    $btop->getLastEntriesNumeroP($l)
						    );
		$sp = $this->sumOverviewFromProject($sp, 
						    $bcm->getLastEntriesNumeroP($l)
						    );		
		if(!is_null($sp))
		  {
		    $sp["project_name"] = $k;
		    $sp["nbProject"] += 1;
		    $res[$k] = $sp;
		  }
	      }
	  }
      }
    return $res;
  }

  public function getStatusFromClientId($clientId)
  {
    $bqueue = new BQueueMaintenance();
    $res = $bqueue->getLastStateFromId($clientId);

    if(is_null($res))
      {
	$btop = new BTopMaintenance();
	$res = $btop->getLastStateFromId($clientId);
	
	if(is_null($res))
	  {
	    $bcm = new BCMMaintenance();
	    $res = $bcm->getLastStateFromId($clientId);
	  }
      }    
    return $res;
  }

  private function sumOverviewFromProject($res, $data)
  {
    if(count($data))
      {
	if(is_null($res))
	  {
	    $res = array("project_name" => null, 
			 "project_id" => null,
			 "nbProject" => 0,
			 "total" => 0, 
			 "OK" => 0,
			 "Checked" => 0,
			 "Warning" => 0,
			 "WarningChecked" => 0,
			 "lastDate" => $data[0]["DateEnd"] );
	  }
	foreach($data as $k => $v)
	  {
	    $res[$v["Etat"]] += 1;
	    $res["total"] += 1;
	  }
      }
    return $res;
  }
}

?>