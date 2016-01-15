<?php

include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/Utils/DateOps.inc");
include_once("BluePHP/BlueSystem/GUI/BSDialogInfo.inc");
include_once("BluePHP/BlueSystem/GUI/BSApplication.inc");
include_once("BluePHP/BTopLocalServer/GlobalProperties.inc");
include_once("BluePHP/BTopLocalServer/RawCounters.inc");
include_once("BluePHP/BTopLocalServer/Location.inc");
include_once("BluePHP/BTopLocalServer/BCLDay.inc");

class DatabaseStatusUI extends BSDialogInfo
{
  public function __construct($id = "", $params = "")
  {
    parent::__construct($id, array("title" => _("Databases status") ), 
			$params);
    parent::setContent("BSDialogInfo");
    $this->build_ui_contents();
  }

  private function index_counts_by_state($counts)
  {
    $resCounts = array( 0, 0, 0, 0 );
    foreach($counts as $v)
      {
	$resCounts[ (int) $v["state"] ] = $v["count"];
      }    
    return $resCounts;
  }

  public function renderContents()
  {
    $contents = $this->getMsg();
    echo $contents;
  }

  protected function build_ui_contents()
  {
    $module = new GlobalProperties();
    $lastPTime = $module->getLastProcessedTimeString();
    //    convertToDBTimeFormat($module->getLastProcessedTimeString());

    $gui = H()->h2(_("General information"));
    $gui->ul( H()->li( _("Last time of processed counting data:") . 
		       " " . convertToDBTimeFormat($lastPTime) )
	      );
    $gui->h2(_("Raw counting database"));

    $rows = H()->tr( H()->th(_("Type") )->th("New")->th("In time")->th("Late")->th("Overfull") );

    $module = new RawCounters();
    $counts = $module->getCountsByState();

    $resCounts = $this->index_counts_by_state($counts["counting"]);

    $rows->tr( H()->td(_("Counting"))
	       ->td($resCounts[0])
	       ->td($resCounts[1])
	       ->td($resCounts[2])
	       ->td($resCounts[3]) );

    $resCounts = $this->index_counts_by_state($counts["numbering"]);

    $rows->tr( H()->td(_("Numbering"))
	       ->td($resCounts[0])
	       ->td($resCounts[1])
	       ->td($resCounts[2])
	       ->td($resCounts[3]) );

    $gui->table( $rows );

    $gui->h2(_("Database of processed data"));

    $module = new Location();
    $locs = $module->getEntries();
    if(count($locs)) 
      {
    	$rows = H()->tr( H()->th(_("Table"))->th(_("Cumul"))->th(_("Exp"))->th(_("LastM"))
			 ->th(_("NbId"))->th(_("Total")) );
    	$module = new BCLDay(substr($lastPTime,0,8));
    	$data = $module->getAllStatus();
    	foreach($data as $k => $v)
	  {
	    if(count($v) and count($v[0]))
	      {
		$row = H()->td($k);
		foreach($v[0] as $l)
		  {
		    $row->td($l);
		  }
		$rows->tr($row);		     
	      }
	  }    
    	$gui->table( $rows );
      } else 
      {
	$gui->p(_("No indicators created"));
      }

    $func = BS_callClassHTML(array( "class" => "BTopLocalServer.DatabaseStatusUI",
				    "mode" => "replace",
				    "render" => $this->getId(),
				    "function" => "renderContents" )
			     );   
    $gui->button(_("Refresh"))->withOnClick($func);
    $this->setMsg($gui);
  }
}

?>
