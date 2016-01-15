<?php

include_once("BluePHP/BlueProjectPHP/Config.inc");
include_once("BluePHP/Utils/ModuleProcessing.inc");

class ComDevis extends ModuleProcessing
{
  private $prefix = "DE";

  public function __construct($sharedCon = false)
  {
    parent::__construct(array("name" => "ComDevis",
			      "keyname" => "id",
			      "sort" => "id" ),
			$sharedCon);
  }

  public function getEntries()
  {
    return $this->Select("id, dateDoc, ComDevis.contact_id as contact_id, concat(contact_first_name, ' ', contact_last_name) as author " . 
			 "from ComDevis, contacts " . 
			 "where contacts.contact_id = ComDevis.contact_id " . 
			 "order by id desc");
  }

  public function generateNewId()
  {
    $ref = "";
    $lastEntry = $this->Select("id from ComDevis order by id desc limit 1");
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

    $filename = glob(DOCROOT . "/1-ENCAISSEMENTS/1_Devis/" . $year . "/" . $elt["id"] . "*.pdf");
    if($filename and is_array($filename) and count($filename))
      {
	$res = $filename[0];
      }
    return $res;
  }

}

?>