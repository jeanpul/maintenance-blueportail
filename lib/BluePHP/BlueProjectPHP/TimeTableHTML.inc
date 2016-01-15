<?php

include_once("BluePHP/BlueProjectPHP/Config.inc");
include_once("BluePHP/BlueProjectPHP/TabularBi.inc");
include_once("BluePHP/BlueProjectPHP/BPLang.inc");
include_once("BluePHP/GUI/Frame.inc");

class TimeTableHTML extends Frame
{
  protected $params = null;

  function __construct($id, $params)
  {
    parent::__construct($id, "timetable", "BluePHP/BlueProjectPHP/templates");
    $this->params = $params;
  }

  function getActivity($params)
  {
    $bplang = new BPLang();
    return $bplang->getCIRYear($params);
  }

  function getTimeTable()
  {
    $res = $this->getActivity($this->params);

    $data = array( "ANNEE", "SALARIE", "CAT", "SUBCAT", "Jours",  
		   "Janvier", "Fevrier", "Mars", "Avril", "Mai", "Juin",
		   "Juillet", "Aout", "Septembre", "Octobre", "Novembre", "Decembre" );
    $ncols = count($data);
    $rows = array();
    foreach($res as $k => $v)
      {    
	if(!$v["cat_name"])
	  {
	    $v["cat_name"] = "ND";
	  }
	$key = $v["year"] . "/" . $v["name"] . "/" . $v["cir"] . "/" . $v["cat_name"];
	if(!isset($rows[$key])) {
	  $rows[$key] = array( 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
	} 
	$rows[$key][0] += $v["total"] / 4.;
	  $rows[$key][$v["month"]] += $v["total"] / 4.;
      }
    foreach($rows as $k => $v)
      {
	$row = explode("/", $k);
	$row[1] = utf8_encode($row[1]);
	$row = array_merge($row, $v);
	$data = array_merge($data, $row);
      }
    $tab = new TabularBi($data, count($rows)+1, $ncols, true, false);
    $tab->setRepresentation($this->params["export"]);
    return $tab;
  }
  
}

?>