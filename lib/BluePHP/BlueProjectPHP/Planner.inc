<?php

include_once("BluePHP/Utils/ModuleProcessing.inc");
include_once("BluePHP/BlueProjectPHP/Calendar.inc");
include_once("BluePHP/BlueProjectPHP/Config.inc");
include_once("BluePHP/BlueProjectPHP/BPLang.inc");
include_once("BluePHP/BlueProjectPHP/Activity.inc");

class Planner extends Activity
{
  public function __construct($dateParams = array(), $sharedCon = null)
  {
    parent::__construct($dateParams, $sharedCon);
    parent::setParams(array( "name" => "planner_action",
			     "keyname" => "action_id",
			     "sort" => "date" ),
		      $sharedCon);
  }

  private function getActivity()
  {
    $res = array();

    $params = $this->getDateParams();
    $year = $params["year"];
    $month = (int) $params["month"];
    $contact_id = $params["contact_id"];
    $quarter = $params["quarter"];
   
    $nday = getDaysInMonth($year, $month);

    for($i = 0; $i < $nday; $i++)
      {
	$day = sprintf("%02d", $i+1);
	$month = sprintf("%02d", $month);
	$date = $year . "-" . $month . "-" . $day;
	$day = strftime("%A", mktime(0, 0, 0, 
				     (int) substr($date, 5, 2), 
				     (int) substr($date, 8, 2), 
				     (int) substr($date, 0, 4)));	    
	$ndata = array( "action_id" => -1, 
			"date" => $date,
			"day" => $day,
			"cid" => $contact_id, 
			"aid1" => -1, "aid2" => -1, "aid3" => -1, "aid4" => -1,
			"cir1" => "NA", "cir2" => "NA", "cir3" => "NA", "cir4" => "NA" , 
			"cat1" => "", "cat2" => "", "cat3" => "", "cat4" => "" );	

	$query = "date_format(planner_action.date, '%Y-%m-%d') as date, dayPeriod, " . 
	  "planner_action.action_id, cir, cat_name from planner_action, " .
	  "action where date_format(planner_action.date,'%Y-%m-%d')='$date' " .
	  "and action.action_id=planner_action.action_id and planner_action.contact_id=$contact_id";
	$result = parent::Select($query);
	if(is_array($result) and count($result) > 0)
	  {
	    for($j = 0; $j < 4 and $j < count($result); $j++)
	      {
		$quarter = $result[$j]["dayPeriod"];
		$ndata["aid" . $quarter] = $result[$j]["action_id"];
		$ndata["cir" . $quarter] = $result[$j]["cir"];
		$ndata["cat" . $quarter] = $result[$j]["cat_name"];
	      }
	  }
	$res[] = $ndata;
      }
    return $res;
  }

  public function getAvailableActions($params)
  {
    $query = "action_project, action_id, cir, cat_name, project_name from action, " . 
      "projects where date='" . $params["date"] . "' " .
      "and contact_id=" . $params["cid"] . " and action_project=project_id";
    return parent::Select($query);    
  }

  public function getEntries()
  {
    $res = $this->getActivity();
    return $res;
  }

  public function delEntry($params)
  {
    $query = "delete from planner_action where contact_id=" . $params["contact_id"] . 
      " and date_format(date, '%Y-%m-%d')='" . $params["date"] . "' and " . 
      "dayPeriod=" . $params["dayPeriod"];
    parent::QueryNoReturn($query);
  }

  public function setEntry($params)
  {
    $query = "replace into planner_action (date, dayPeriod, action_id, contact_id) values(" . 
      "'" . $params["date"] . " 00:00:00'," . $params["dayPeriod"] . "," . $params["action_id"] . 
      "," . $params["contact_id"] . ")";
    parent::QueryNoReturn($query);
  }

  public function getMonthStats($params)
  {
    $bplang = new BPLang();
    $res = $bplang->getPlannerMonthTotal($params);
    
    $sumWorked = 0;
    $sumRD = 0;
    $sumFilled = 0;

    array_walk($res, function($val, $cat) use ($bplang, &$sumRD, &$sumWorked, &$sumFilled) 
	       { 
		 if($bplang->isRDCat($cat)) { $sumRD += $val; } 
		 if($bplang->isWorkingCat($cat)) { $sumWorked += $val; }
		 $sumFilled += $val;
	       });
    
    $RDrate = $sumWorked > 0 ? round($sumRD/$sumWorked*100) : 0;

    $nday = getDaysInMonth((int) $params["year"], (int) $params["month"]);
    $nWorkable = 0;
    for($i = 1; $i <= $nday; $i++)
      {
	$date = $params["year"] . "-" . sprintf("%02d", $params["month"]) . "-" . sprintf("%02d", $i);
	if(!$bplang->isDayOff($date))
	  {
	    $nWorkable++;
	  }
      }

    return array( "sumWorked" => $sumWorked,
		  "nWorkable" => $nWorkable,
		  "sumRD" => $sumRD,
		  "sumFilled" => $sumFilled,
		  "RDrate" => $RDrate );
  }
}

?>