<?php

include_once("BluePHP/BlueProjectPHP/Config.inc");
include_once("BluePHP/Utils/ModuleProcessing.inc");

class Livraison extends ModuleProcessing
{
  private $prefix = "BL";

  public function __construct($sharedCon = false)
  {
    parent::__construct(array( "name" => "livraison",
			       "keyname" => "livraison_id",
			       "sort" => "dateSend" ),
			$sharedCon);
  }

  protected function setPrefix($p)
  {
    $this->prefix = $p;
  }

  function getEntries()
  {
    return parent::Select("livraison_id, ref, commande, ComCommandes.project_id as project_id, " . 
			  "livraison.comments as comments, livraison.product_id as product_id, " .
			  "project_name, product_name, dateSend, livraison.isValid as isValid from " . 
			  "livraison, ComCommandes, projects, products where " . 
			  "livraison.commande=ComCommandes.id and " . 
			  "ComCommandes.project_id=projects.project_id and " . 
			  "livraison.product_id=products.product_id order by dateSend");
  }

  public function generateNewId()
  {
    $ref = "";
    $lastEntry = $this->Select("ref from livraison where left(ref,3)='" . $this->prefix . "' order by ref desc limit 1");
    if($lastEntry)
      {
	$ref = $this->prefix . sprintf("%04d", (int) substr($lastEntry[0]["ref"], 3) + 1);
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
    $year = substr($elt["dateSend"], 0, 4);

    $filename = glob(DOCROOT . "/1-ENCAISSEMENTS/4_Livraison/" . $year . "/" . $elt["ref"] . "*.pdf");
    if($filename and is_array($filename) and count($filename))
      {
	$res = $filename[0];
      }
    return $res;
  }


}

?>

