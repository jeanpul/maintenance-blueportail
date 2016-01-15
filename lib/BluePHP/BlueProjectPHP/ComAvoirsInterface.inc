<?php

include_once("BluePHP/BlueProjectPHP/ComAvoirs.inc");
include_once("BluePHP/BlueProjectPHP/ComFacturesInterface.inc");

class ComAvoirsInterface extends ComFacturesInterface
{
    public function __construct($id = "", $params = array())
    {
      parent::__construct($id, $params);
      parent::setModule(new ComAvoirs());
      parent::setTableParams( array( "titles" => array( _("Ref"), _("Cmd"), _("Date"), _("Montant HT (€)"), _("Client"), _("isValid"), _("ToPay") ),
				     "cols" => array( "id", "cmd_id", "dateDoc", "montantHT", "clientName", "isValid", "datePay" ),
				     "cbId" => $this->getId(),
				     "cbModule" => "bp_com.avoirs")
			      );
      $this->setCreateLinkTitle(_("Create a new Avoir"));      
      $bs = $this->getDialogTable();
      $bs->setTitle("Avoir");
      parent::setModuleName("BlueProjectPHP");			      
    }    
}

?>