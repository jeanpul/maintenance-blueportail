<?php

include_once("BluePHP/BlueProjectPHP/Config.inc");
include_once("BluePHP/Utils/ModuleProcessing.inc");
include_once("BluePHP/BlueProjectPHP/ComPaiements.inc");

class ComFournisseurs extends ModuleProcessing
{
  private $prefix = "SUP";

  public function __construct($sharedCon = false)
  {
    parent::__construct(array("name" => "ComFournisseurs",
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
    return $this->generic_get_entries(" and ComFournisseurs.isValid=1");
  }

  private function generic_get_entries($where = "")
  {    
    return $this->Select("ComFournisseurs.id as id, cmd_id, ComFournisseurs.dateDoc as dateDoc, ComFournisseurs.montantHT as montantHT, " . 
			 "C1.company_name as clientName, C2.company_name as supplierName, ComFournisseurs.refClient as refClient, ComFournisseurs.isValid as isValid " . 
			 "from ComFournisseurs, ComCommandes, companies as C1, companies as C2 " . 
			 "where ComFournisseurs.cmd_id=ComCommandes.id and C1.company_id=ComCommandes.company_id and C2.company_id=ComFournisseurs.company_id" . 
			 $where . " order by dateDoc asc");
  }

  public function generateNewId()
  {
    $ref = "";
    $lastEntry = $this->Select("id from ComFournisseurs where left(id,3)='" . $this->prefix . "' order by id desc limit 1");
    if($lastEntry)
      {
	$ref = $this->prefix . sprintf("%04d", (int) substr($lastEntry[0]["id"], 3) + 1);
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

    $filename = glob(DOCROOT . "/2-DECAISSEMENTS/4-Facture/" . $year . "/" . $elt["id"] . "*.pdf");
    if($filename and is_array($filename) and count($filename))
      {
	$res = $filename[0];
      }
    return $res;
  }
}

?>