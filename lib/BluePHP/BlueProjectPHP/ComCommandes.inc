<?php

include_once("BluePHP/BlueProjectPHP/Config.inc");
include_once("BluePHP/Utils/ModuleProcessing.inc");

class ComCommandes extends ModuleProcessing
{
  public function __construct($sharedCon = false)
  {
    parent::__construct(array("name" => "ComCommandes",
			      "keyname" => "id",
			      "sort" => "id" ),
			$sharedCon);
  }

  public function getValidEntries()
  {
    return $this->generic_get_entries(" and isValid=1");
  }

  public function getEntries()
  {
    return $this->generic_get_entries();
  }

  private function generic_get_entries($where = "")
  {
    return $this->Select("id, dateDoc, montantHT, company_name as clientName, refClient, isExport, isValid, project_id " . 
			 "from ComCommandes, companies " . 
			 "where companies.company_id = ComCommandes.company_id" . 
			 $where . " order by id desc");
  }

  public function generateNewId()
  {
    $ref = "";
    $curYear = strftime("%y");
    $lastEntry = $this->Select("id from ComCommandes where left(right(id, 4),2)='$curYear' order by id desc limit 1");
    if($lastEntry)
      {
	$ref = "CMD" . $curYear . sprintf("%02d", (int) substr($lastEntry[0]["id"], -2) + 1);
      }
    else
      {
	$ref = "CMD" . $curYear . "01";
      }
    return $ref;
  }

  public function getFileName($elt)
  {
    $res = false;
    $year = substr($elt["dateDoc"], 0, 4);
    $filename = glob(DOCROOT . "/1-ENCAISSEMENTS/2_CMD/" . $year . "/" . $elt["id"] . "*.pdf");    
    if($filename and count($filename))
      {
	$res = $filename[0];
      }
    
    return $res;
  }

}

?>