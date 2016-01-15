<?php

include_once("BluePHP/BlueProjectPHP/Config.inc");
include_once("BluePHP/Utils/ModuleProcessing.inc");
include_once("BluePHP/BlueProjectPHP/ComPaiements.inc");

class ComFactures extends ModuleProcessing
{
  private $prefix = "FA";

  public function __construct($sharedCon = false)
  {
    parent::__construct(array("name" => "ComFactures",
			      "keyname" => "id",
			      "sort" => "dateDoc" ),
			$sharedCon);
  }

  protected function setPrefix($p)
  {
    $this->prefix = $p;
  }

  public function getEntries()
  {    
    return $this->generic_get_entries();
  }

  public function getValidEntries()
  {
    return $this->generic_get_entries(" and ComFactures.isValid=1");
  }

  private function generic_get_entries($where = "")
  {    
    return $this->Select("ComFactures.id as id, cmd_id, ComFactures.dateDoc as dateDoc, ComFactures.montantHT as montantHT, " . 
			 "montantTTC, company_name as clientName, datePay, ComFactures.isValid as isValid " . 
			 "from ComFactures, ComCommandes, companies " . 
			 "where ComFactures.cmd_id=ComCommandes.id and companies.company_id=ComCommandes.company_id and " . 
			 "left(ComFactures.id,2)='" . $this->prefix . "'" . 
			 $where . " order by dateDoc asc");
  }

  public function generateNewId()
  {
    $ref = "";
    $lastEntry = $this->Select("id from ComFactures where left(id,2)='" . $this->prefix . "' order by id desc limit 1");
    if($lastEntry)
      {
	$ref = $this->prefix . sprintf("%04d", (int) substr($lastEntry[0]["id"], 2) + 1);
      }
    else
      {
	$ref = $this->prefix . "0000";
      }
    return $ref;
  }

  public function getFileName($elt)
  {
    $res = false;
    $year = substr($elt["dateDoc"], 0, 4);

    $filename = glob(DOCROOT . "/1-ENCAISSEMENTS/3_Facture client_avoir/" . $year . "/" . $elt["id"] . "*.pdf");
    if($filename and is_array($filename) and count($filename))
      {
	$res = $filename[0];
      }
    return $res;
  }

  public function newEntry($params)
  {
    $res = parent::newEntry($params);
    if($res)
      {
	if($params["isValid"] == 1)
	  {
	    $mp = new ComPaiements($this);
	    $res = $mp->newEntry( array( "facture_id" => $params["id"], 
					 "datePayWanted" => $params["datePay"],
					 "datePayReal" => "NULL", 
					 "montantTTC" => "NULL",
					 "charges" => "NULL",
					 "refBank" => "",
					 "comments" => "" ) );
	  }
      }
    return $res;
  }

  public function setEntry($params)
  {  
    $elt = $this->getEntry($params);
    $res = parent::setEntry($params);
    if($res and $elt)
      {    
	if($elt["isValid"] != $params["isValid"])
	  {
	    $mp = new ComPaiements($this);
	    if($params["isValid"] == 1)	      
	      {
		$res = $mp->newEntry( array( "facture_id" => $params["id"], 
					     "datePayWanted" => $params["datePay"],
					     "datePayReal" => "NULL", 
					     "montantTTC" => "NULL",
					     "charges" => "NULL",
					     "refBank" => "",
					     "comments" => "" ) );
	      }
	    else
	      {
		$res = $mp->delEntry( array( "facture_id" => $params["id"] ) );
	      }
	  }
      }
    return $res;
  }

  public function delEntry($params)
  {
    $res = parent::delEntry($params);
    if($res)
      {
	$mp = new ComPaiements($this);
	$res = $mp->delEntry( array( "facture_id" => $params["id"] ) );
      }
    return $res;
  }
}

?>