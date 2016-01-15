<?php

include_once("BluePHP/BlueProjectPHP/Config.inc");
include_once("BluePHP/BlueProjectPHP/TabularBi.inc");
include_once("BluePHP/BlueProjectPHP/BPLang.inc");
include_once("BluePHP/GUI/Frame.inc");

class SummaryTimeTableHTML extends Frame
{
  protected $params = null;

  function __construct($id, $params)
  {
    parent::__construct($id, "summarytimetable", "BluePHP/BlueProjectPHP/templates");
    $this->params = $params;
  }

  function getDiffFontStyle($number)
  {
    return $number < 0 ? "<font color='red'>$number</font>" : "<font color='green'>$number</font>";
  }

  function getSummaryTimeTable()
  {
    $str = "";
    $nbHoursYear = 1607;
    
    $decls = array( "4.2.1" => array( "cost" => 10754.85, "hours" => round($nbHoursYear * (3/12.))),
		    "4.2.2" => array( "cost" => 46612.35, "hours" => round($nbHoursYear * (11/12.))),
		    "4.2.3" => array( "cost" => 46612.35, "hours" => round($nbHoursYear * (11/12.))),
		    "4.2.4" => array( "cost" => 38007, "hours" => round($nbHoursYear * (9/12.))),
		    "4.2.5" => array( "cost" => 41592, "hours" => round($nbHoursYear * (10/12.))),
		    "4.4.7" => array( "cost" => 34118, "hours" => round($nbHoursYear * (6/12.))),
		    "5.1.1" => array( "cost" => 3584.95, "hours" =>  round($nbHoursYear * (1/12.))),
		    "5.1.2" => array( "cost" => 3584.95, "hours" =>  round($nbHoursYear * (1/12.))),
		    "5.1.3" => array( "cost" => 3584.95, "hours" =>  round($nbHoursYear * (1/12.))));
    
    $costs = array( "Fabien" => 29.2544, //31.7055,
		    "Sebastien" => 24.3354, //26.5370,
		    "Samuel" => 15.7710,
                    "Thomas" => 15.7710 ); // 21.4025 );

    $worktimeDay = 8;
    
    $bplang = new BPLang();
    $rows = $bplang->getCIRTotal($this->params);
    
    $data = array( "Cat", "Cir", "Decl Hours", "Decl Cost", "Current Hours", "Current Cost", "Diff Hours", "Diff Cost" );
    $ncols = count($data);
    
    // first run through results to compute prices and days
    $totals = array();
    foreach($rows as $k => $v)
      {
	if(!isset($totals[$v["cat_name"]]))
	  {
	    $totals[$v["cat_name"]] = $v;
	    $totals[$v["cat_name"]]["cost"] = 0.;
	    $totals[$v["cat_name"]]["hours"] = 0;
	  }      
	$nbdays = $v["total"] / 4.;
	$nbhours = $nbdays * $worktimeDay;
	$cost = $nbhours * $costs[$v["name"]];
	
	$totals[$v["cat_name"]]["cost"] += $cost;
	$totals[$v["cat_name"]]["hours"] += $nbhours;
      }
    
    $tt = array( "titles" => array( "Decl Hours", "Decl Cost", 
				    "Current Hours", "Current Cost", "Diff Hours", "Diff Cost" ), 
		 "data" => array( 0, 0, 0, 0, 0, 0 ) );
    
    // run second time to compute totals
    foreach($totals as $k => $v)
      {
	$decl = array( "hours" => 0, "cost" => 0 );
	if(isset($decls[$v["cat_name"]]))
	  {
	    $decl = $decls[$v["cat_name"]];
	  }
	$diffHours = $v["hours"] - $decl["hours"];
	$diffCost = $v["cost"] - $decl["cost"];
	$row = array( $v["cat_name"], $v["cir"], 
		      $decl["hours"], $decl["cost"], 
		      $v["hours"], $v["cost"], 
		      $this->getDiffFontStyle($diffHours), 
		      $this->getDiffFontStyle($diffCost) ); 
	$data = array_merge($data, $row);
	
	$tt["data"][0] += $decl["hours"];
	$tt["data"][1] += $decl["cost"];
	$tt["data"][2] += $v["hours"];
	$tt["data"][3] += $v["cost"];
	$tt["data"][4] += $diffHours;
	$tt["data"][5] += $diffCost;
      }
    
    $res = array();

    $tab = new TabularBi($data, count($totals) + 1, $ncols, true, false);
    $tab->setRepresentation($this->params["export"]);
    
    $res[] = $tab;
    
    $tt["data"][4] = $this->getDiffFontStyle($tt["data"][4]);
    $tt["data"][5] = $this->getDiffFontStyle($tt["data"][5]);
    $tab = new TabularBi(array_merge($tt["titles"], $tt["data"]), 2, count($tt["titles"]), true, false);
    $tab->setRepresentation($this->params["export"]);

    $res[] = $tab;
    
    return $res;
  }
}

?>