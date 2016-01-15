<?php

include_once("BluePHP/BlueProjectPHP/HTML.inc");

class BPTable
{
  var $columnNames = array();
  var $rowData = array();

  function BPTable($rowData, $columnNames = array())
    {
      $this->rowData = $rowData;
      $this->columnNames = $columnNames;
    }

  function render()
  {
    $str = "<table>\n";
    if(count($this->columnNames))
      {
	$str .= generateROW($this->columnNames, "th");
      }
    if(count($this->rowData))
      {
	foreach($this->rowData as $k => $v)
	  {
	    if(count($v))
	      {
		$str .= generateROW($v);
	      }
	  }
      }
    $str .= "</table>\n";
    return $str;
  }
}

?>