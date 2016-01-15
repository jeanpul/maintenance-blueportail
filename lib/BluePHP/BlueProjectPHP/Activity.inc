<?php

include_once("BluePHP/Utils/ModuleProcessing.inc");
include_once("BluePHP/BlueProjectPHP/Calendar.inc");
include_once("BluePHP/BlueProjectPHP/Config.inc");

class Activity extends ModuleProcessing
{
  private $dateParams = array( "year" => 2013,
			       "month" => 1,
			       "contact_id" => 26,
			       "quarter" => 1 );

  public function __construct($dateParams, $sharedCon = null)
  {
    parent::setParams(array( "name" => "action",
			     "keyname" => "action_id",
			     "sort" => "date" ),
		      $sharedCon);
    $curDate = getdate();
    $this->dateParams = array( "year" => $curDate["year"], 
			       "month" => $curDate["mon"], 
			       "contact_id" => 26, 
			       "quarter" => 1 );
    $this->dateParams = array_merge($this->dateParams, $dateParams);
  }

  public function setDateParams($p)
  {
    $this->dateParams = array_merge($this->dateParams, $p);
  }

  public function getDateParams()
  {
    return $this->dateParams;
  }

  public function getEntries()
  {
    $res = array();
    if($this->dateParams["month"] == -1)
      {
	for($m = 1; $m <= 12; $m++)
	  {
	    $this->dateParams["month"] = $m;
	    $res = array_merge($res,$this->getActivity());
	  }
      }
    else
      {
	$res = array_merge($res,$this->getActivity());
      }

    return $res;
  }

  public function getEntryForCIR($params)
  {
    return parent::Select("* from action where date(date)='" . $params["date"] . "' and cir='" . $params["cir"]
			  . "' and contact_id=" . $params["contact_id"]);
  }

  private function getActivity()
  {
    $res = array();
    
    $year = (int) $this->dateParams["year"];
    $month = (int) $this->dateParams["month"];
    $contact_id = $this->dateParams["contact_id"];
    $quarter = $this->dateParams["quarter"];
    
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
			"com1" => "", "com2" => "", "com3" => "", "com4" => "",
			"fam1" => "", "fam2" => "", "fam3" => "", "fam4" => "",
			"cat1" => "", "cat2" => "", "cat3" => "", "cat4" => "" );
	$query = "action_id, date_format(date,'%Y-%m-%d') as date, cir, action.comment as comment, familly.familly_name as fam, action_cat.name as cat from action, familly, action_cat" .
	  " where date_format(date,'%Y-%m-%d')='$date' and contact_id=$contact_id and familly.familly_id=action_familly and action_cat.cat_id=action.cat_id";
	$result = $this->Select($query);
	if(is_array($result) and count($result) > 0)
	  {
	    $j = 0;
	    for(; $j < 4 and $j < count($result); $j++)
	      {
		$quarter = $j+1;
		$ndata["aid" . $quarter] = $result[$j]["action_id"];
		$ndata["cir" . $quarter] = $result[$j]["cir"];
		$ndata["com" . $quarter] = $result[$j]["comment"];
		$ndata["fam" . $quarter] = $result[$j]["fam"]; 
		$ndata["cat" . $quarter] = $result[$j]["cat"];
	      }
	  }
	$res[] = $ndata;
      }

    return $res;
  }


}

?>