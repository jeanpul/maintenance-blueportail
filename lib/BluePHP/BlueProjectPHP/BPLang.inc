<?php

include_once("BluePHP/Utils/DBConnect.inc");
include_once("BluePHP/BlueProjectPHP/Calendar.inc");

class BPLang
{
  // DB connexion
  var $con = false;
  
  // R&D categories
  var $rdCat = array( "RD1", "RD2", "DEV" );

  // day off categories
  var $notWorkingCat = array( "RTT", "MAL", "CP", "SS");

  function BPLang()
    {
    }

  function connect()
  {
    if(!$this->con)
      {
	$this->con = new DBConnect(CONFIGDBTYPE, 
				   CONFIGDBHOST, 
				   CONFIGDBNAME,
				   CONFIGDBUSER, 
				   CONFIGDBPASSWD);
	$this->con->connectToDB();
      }
    return $this->con;
  }

  function isRDCat($cat)
  {
    return in_array($cat, $this->rdCat);
  }

  function isWorkingCat($cat)
  {
    return !in_array($cat, $this->notWorkingCat);
  }

  function getSalNames($params)
  {
    $this->connect();

    $salNames = array();
    $query = "select contacts_projects.contact_id as contact_id, " . 
      "contact_first_name, contact_last_name " .
      "from contacts_projects, contacts where " .
      "contacts_projects.contact_id=contacts.contact_id and " . 
      "contacts_projects.project_id=223";
    $result = $this->con->query($query);
    
    while($obj = $result->nextAssoc())
      {
	$n = $obj["contact_first_name"] . " " . $obj["contact_last_name"];
	$salNames[] = array( "idx" => $obj["contact_id"],
			     "name" => $n );      
      }
    $result->freeResult();
    return $salNames;
  }

  function getSalCompanyNames($params)
  {
    $this->connect();

    $salNames = array();
    $compName = $params["compName"];
    $query = "select contacts.contact_id as contact_id, " . 
      "contact_first_name, contact_last_name " .
      "from contacts, companies where " .
      "company_name=\"$compName\" and contacts.company_id=companies.company_id";
    $result = $this->con->query($query);
    
    while($obj = $result->nextAssoc())
      {
	$n = $obj["contact_first_name"] . " " . $obj["contact_last_name"];
	$salNames[] = array( "idx" => $obj["contact_id"],
			     "name" => $n );      
      }
    $result->freeResult();
    return $salNames;
  }

  // yyyy-mm-dd
  function isWeekEnd($date)
  {
    $dateUnix = mktime(0, 0, 0, 
		       substr($date, 5, 2), 
		       substr($date, 8,2), 
		       substr($date, 0, 4));
    $date = getdate($dateUnix);
    return $date["wday"] == 0 or $date["wday"] == 6;
  }

  function isDayOff($date)
  {    
    $isDayOff = false;
    $this->connect();
    $query = "select date from action where date(date)='$date' and action_project=0";
    $result = $this->con->query($query);
    if($result)
      {
	if($obj = $result->nextAssoc())
	  {
	    $isDayOff = true;
	  }
      }
    return $this->isWeekEnd($date) or $isDayOff;
  }

  function getPlannerMonthTotal($params)
  {
    $this->connect();

    $res = array();

    $year = $params["year"];
    $month = $params["month"];
    $contact_id = $params["contact_id"];

    $query = "select cir as cat, count(*) as nb from planner_action, action " .
      "where date_format(planner_action.date,'%Y-%m')='$year-$month' and " .
      "planner_action.contact_id=$contact_id and " .
      "action.action_id=planner_action.action_id group by cir";

    $result = $this->con->query($query);

    while($obj = $result->nextAssoc())
      {
	$nb = $obj["nb"] / 4.;
	$cat = $obj["cat"];
	$res["$cat"] = $nb;
      }
    $result->freeResult();
    return $res;
  }

  function getCIRYear($params)
  {
    $res = array();
    $this->connect();
    
    $cat = $params["cir"];

    $dateLimit = "";
    if(isset($params["dateStart"]) and isset($params["dateEnd"]))
      {
	$dateLimit = " and date_format(planner_action.date, '%Y%m%d') >= '" . $params["dateStart"] . "' and " .
	  "date_format(planner_action.date, '%Y%m%d') <= '" . $params["dateEnd"] . "'";
      }    

    $project = isset($params["action_project"]) ? "and action_project=" . $params["action_project"] : "";

    $query = "select contact_first_name as name, count(quarter) as total, " .
      "year(planner_action.date) as year, " .
      "month(planner_action.date) as month, " .
      "cat_name, cir from action, planner_action, " . 
      "contacts where " .  
      "planner_action.contact_id=contacts.contact_id and " . 
      "action.action_id=planner_action.action_id and cir=\"$cat\" $dateLimit $project " .
      "group by name, year, month, cat_name";
    $result = $this->con->query($query);
    while($obj = $result->nextAssoc())
      {
	$res[] = $obj;
      }
    $result->freeResult();
    return $res;
  }	  

  function getCIRTotal($params)
  {
    $res = array();
    $this->connect();
    
    $cat = $params["cir"];

    $dateLimit = "";
    if(isset($params["dateStart"]) and isset($params["dateEnd"]))
      {
	$dateLimit = " and date_format(planner_action.date, '%Y%m%d') >= '" . $params["dateStart"] . "' and " .
	  "date_format(planner_action.date, '%Y%m%d') <= '" . $params["dateEnd"] . "'";
      }    
    
    $query = "select contact_first_name as name, count(quarter) as total, " .
      "cat_name, cir from contacts, action, planner_action where " . 
      "planner_action.contact_id=contacts.contact_id and " . 
      "action.action_id=planner_action.action_id and cir=\"$cat\" $dateLimit " .
      "group by name, cir, cat_name";
    $result = $this->con->query($query);
    while($obj = $result->nextAssoc())
      {
	$res[] = $obj;
      }
    $result->freeResult();
    return $res;
  }

  function getPlannerMonth($params)
  {
    $res = array();

    $this->connect();

    $year = $params["year"];
    $month = sprintf("%02d", $params["month"]);
    $contact_id = $params["contact_id"];

    $data = array();
    $query = "select date_format(planner_action.date, '%Y-%m-%d') as date, dayPeriod, " . 
      "concat(planner_action.action_id, '/', cir, '/', cat_name) as action from planner_action, " .
      "action where date_format(planner_action.date,'%Y-%m')='$year-$month' " .
      "and action.action_id=planner_action.action_id and planner_action.contact_id=$contact_id";
    $result = $this->con->query($query);
    while($obj = $result->nextAssoc())
      {
	$data[$obj["date"]][$obj["dayPeriod"]] = $obj;
      }
    $result->freeResult();

    // fill in missing data
    $nday = getDaysInMonth((int) $year, (int) $month);
    for($i = 0; $i < $nday; $i++)
      {
	$day = sprintf("%02d", $i+1);
	$date = $year . "-" . $month . "-" . $day;
	if(isset($data[$date]))
	  {
	    $res[$date] = $data[$date];
	  }
	$res[$date]["date"] = $date;
	$res[$date]["day"] = strftime("%A", mktime(0, 0, 0, $month, $day, $year));
      }
    return $res;  
  }

  function getCompaniesName($params = null)
  {
    return $this->getModuleEntries(array( "fields" => "company_id, company_name",
					  "name" => "companies",
					  "sort" => "company_name" ));
  }

  function getModuleEntries($params)
  {
    $res = array();
    $this->connect();
    $query = "select ";
    if(isset($params["fields"]) and $params["fields"])
      {
	$query .= $params["fields"];
      }
    else
      {
	$query .= "*";
      }

    $query .= " from " . $params["name"];
    if(isset($params["sort"]) and $params["sort"])
      {
	$query .= " order by " . $params["sort"];
      }

    $result = $this->con->query($query);
    while($obj = $result->nextAssoc())
      {
	$res[] = $obj;
      }
    $result->freeResult();
    
    return $res;    
  }

  function getModuleEntry($params)
  {
    $res = array();
    $this->connect();
    if(isset($params[$params["keyname"]]))
      {
	$query = "SELECT * FROM " . $params["name"] . 
	  " WHERE " . $params["keyname"] . "=" . $params[$params["keyname"]];
	$result = $this->con->query($query);
	$res = $result->nextAssoc();
	$result->freeResult();
      }
    else
      {
	$query = "select * from " . $params["name"] . " limit 1";
	$result = $this->con->query($query);
	$res = $result->nextAssoc();
	$result->freeResult();
	if(!$res)
	  {
	    $res = array_merge($this->getModuleParams($params),
			       $params);
	  }
	$res = array_merge($res, $params);
      }
    return $res;
  }

  function getModuleParams($params)
  {
    $res = array();
    $this->connect();
    $query = "SHOW COLUMNS FROM " . $params["name"];
    $result = $this->con->query($query);
    while($col = $result->nextAssoc())
      {
	$res[$col["Field"]] = null; // empty parameter
      }
    $result->freeResult();
    return $res;
  }
    

    function newModuleEntry($params)
    {
      $data = array_intersect_key($params,
				  $this->getModuleParams( array( "name" => $params["name"] )));
      $cols = "(";
      $values = "values (";
      $lastkey = end($data);
      foreach($data as $k => $v)
	{
	  $cols .= $k;
	  $values .= "\"" . $v . "\"";
	  if($v != $lastkey)
	    {
	      $cols .= ",";
	      $values .= ",";
	    }
	}
      $cols .= ")";
      $values .= ")";

      $this->connect();
      $query = "insert into " . $params["name"] . "$cols $values";
      $result = $this->con->query($query);
      return $result;
    }

    function setModuleEntry($params)
    {
      $data = array_intersect_key($params,
				  $this->getModuleParams( array( "name" => $params["name"] )));
      $updatepart = "";
      $wherepart = "where " . $params["keyname"] . "=" . $data[$params["keyname"]];
      $lastkey = end($data);
      foreach($data as $k => $v)
	{
	  $updatepart .= $k . "=" . "\"" . $v . "\"";
	  if($v != $lastkey)
	    {
	      $updatepart .= ",";
	    }
	}
      $this->connect();
      $query = "update " . $params["name"] . " set " . 
	$updatepart . " " . $wherepart;
      $result = $this->con->query($query);
      return $result;
    }

    function delModuleEntry($params)
    {
      $this->connect();
      $query = "delete from " . $params["name"] . " where " . $params["keyname"] .
	"=" . $params[$params["keyname"]];
      $result = $this->con->query($query);
      return $result;
    }

    function Select($query)
    {
      $res = array();
      $this->connect();
      $result = $this->con->query("select " . $query);
      while($obj = $result->nextAssoc())
      {
	$res[] = $obj;
      }
      $result->freeResult();
      return $res;

    }
}

?>
