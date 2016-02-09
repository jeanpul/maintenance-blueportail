<?php

class IndicatorBase
{
  private $clang;
  private $tblName;

  protected function __construct($clang, $tblName)
  {
    $this->clang = $clang;
    $this->tblName = $tblName;
  }

  public function process($params = "")
  {
    $params["valueKind"] = "counting";
    return $this->clang->getValues($params);
  }

  function getValues($idp, $timestamp, $hstart, $hend)
  {
    $data = null;
    $dataDB = $this->clang->accessDataDB("minute", $timestamp);
    if($dataDB !== FALSE)
      {
	$query = "select * from " . $this->tblName . "_MINUTE where id=$idp and " . 
	  "strftime('%H%M', start) >= '$hstart' and " . 
	  "strftime('%H%M', start) < '$hend'";
	$data = $this->clang->queryDataDB($query);
      }
    return $data;
  }

  function deleteValues($idp, $timestamp, $hstart, $hend)
  {
    $dataDB = $this->clang->accessDataDB("minute", $timestamp);
    if($dataDB !== FALSE)
      {
	$query = "delete from " . $this->tblName . "_MINUTE where id=$idp and " . 
	  "strftime('%H%M', start) >= '$hstart' and " . 
	  "strftime('%H%M', start) < '$hend'";
	$this->clang->queryNoReturnDataDB($query);
	// update by day cumulated data
	$query = "select sum(value0) as s0, sum(value1) as s1, " .
	  "sum(nbcumul0) as n0, sum(nbcumul1) as n1, " .
	  "sum(nbexpected0) as e0, sum(nbexpected1) as e1, " .
	  "strftime('%Y-%m-%d 00:00:00', start) as start " .
	  "from " . $this->tblName . "_MINUTE where id=$idp";
	$data = $this->clang->queryDataDB($query);
	if(is_array($data))
	  {
	    $data = array_pop($data);
	    $dataDB = $this->clang->accessDataDB("day", $timestamp);
	    $query = "replace into " . $this->tblName . "_DAY values (" . 
	      $idp . ",'" . $data["start"] . "'," . $data["s0"] . "," .
	      $data["n0"] . "," . $data["e0"] . "," . $data["s1"] . "," . 
	      $data["n1"] . "," . $data["e1"] . ")";
	    $this->clang->queryNoReturnDataDB($query);
	  }
      }
  }
}

?>
