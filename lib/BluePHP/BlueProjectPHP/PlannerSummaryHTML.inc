<?php

include_once("BluePHP/BlueProjectPHP/Config.inc");
include_once("BluePHP/BlueProjectPHP/HTML.inc");
include_once("BluePHP/BlueProjectPHP/BPTable.inc");
include_once("BluePHP/BlueProjectPHP/BPLang.inc");
include_once("BluePHP/GUI/Frame.inc");
include_once("BluePHP/Utils/DBConnect.inc");

class PlannerSummaryHTML extends Frame
{
  protected $params = null;

  function __construct($id, $params)
  {
    parent::__construct($id, "plannersummary", "BluePHP/BlueProjectPHP/templates"); 
    $curDate = getdate();    
    $this->params = array( "year" => $curDate["year"], 
			   "month" => $curDate["mon"], 
			   "contact_id" => 30, 
			   "quarter" => 1, 
			   "export" => "html-table" );
    $this->params = array_merge($this->params, $params);
  }

  function makeSummary()
  {
    $bplang = new BPLang();
    $res = $bplang->getPlannerMonthTotal($this->params);
    
    $colNames = array( array_keys($res) );
    $rowData = array( array_values($res) );
    $sumWorked = 0;
    $sumRD = 0;
    
    array_walk($res, function($val, $cat) use ($bplang, &$sumRD, &$sumWorked) 
	       { 
		 if($bplang->isRDCat($cat)) { $sumRD += $val; } 
		 if($bplang->isWorkingCat($cat)) { $sumWorked += $val; }
	       });
    
    $table = new BPTable( $rowData, $colNames[0] );
    $str = $table->render();
    
    $RDrate = $sumWorked > 0 ? round($sumRD/$sumWorked*100) : 0;
    $color = "green";
    if($RDrate < 50)
      {
	$color = "red";
      }
    $table = new BPTable( array( array( "Total jours travaillés dans le mois", $sumWorked ),
				 array( "Total des temps passés à la R&D", $sumRD . " <font color='$color'>(" . (int) $RDrate . "%)</font>" ) ));
    $str .= $table->render();
    return $str;
  }
  
}

?>