<?php

include_once("BluePHP/BlueProjectPHP/Config.inc");
include_once("BluePHP/Utils/ModuleProcessing.inc");

class ComPaiements extends ModuleProcessing
{
  public function __construct($sharedCon = false)
  {
    parent::__construct(array("name" => "ComPaiements",
			      "keyname" => "facture_id",
			      "sort" => "datePayWanted" ),
			$sharedCon);
  }

  public function getEntry($params)
  {    
    $elts = $this->Select("facture_id, ComFactures.datePay as datePayWanted, datePayReal, " . 
			  "ComPaiements.montantTTC as montantTTC, ComFactures.montantTTC as montantTTCFac, " . 
			  "charges, refBank, isPayed, company_name, ComPaiements.comments as comments " .
			  "from ComPaiements, ComFactures, ComCommandes, companies " . 
			  "where facture_id=ComFactures.id and ComFactures.cmd_id=ComCommandes.id and " . 
			  "ComCommandes.company_id=companies.company_id and facture_id=\"" . $params["facture_id"] . "\" order by datePayReal desc");
    return $elts[0];
  }

  public function getEntries()
  {    
    return $this->Select("facture_id, ComFactures.datePay as datePayWanted, datePayReal, " . 
			 "ComPaiements.montantTTC as montantTTC, ComFactures.montantTTC as montantTTCFac, " . 
			 "charges, refBank, isPayed, company_name " .
			 "from ComPaiements, ComFactures, ComCommandes, companies " . 
			 "where facture_id=ComFactures.id and ComFactures.cmd_id=ComCommandes.id and " . 
			 "ComCommandes.company_id=companies.company_id order by datePayReal desc");
  }

}

?>