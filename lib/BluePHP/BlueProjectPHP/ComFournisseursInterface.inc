<?php

include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/BlueSystem/GUI/BSModuleInterface.inc");
include_once("BluePHP/BlueProjectPHP/Companies.inc");
include_once("BluePHP/BlueProjectPHP/ComCommandes.inc");
include_once("BluePHP/BlueProjectPHP/ComFournisseurs.inc");

class ComFournisseursInterface extends BSModuleInterface
{
    public function __construct($id = "", $params = array())
    {
      parent::__construct($id, $params);
      parent::setModule(new ComFournisseurs());
      parent::setTableParams( array( "titles" => array( _("Ref"), _("Date"), _("Ref supplier"), _("Supplier"), _("Cmd"), _("Client"), _("Montant HT (€)"), _("isValid") ),
				     "cols" => array( "id","dateDoc",  "refClient", "supplierName", "cmd_id", "clientName", "montantHT", "isValid" ),
				     "cbId" => $this->getId(),
				     "cbModule" => "bp_com.fournisseurs.edit",
				     "viewLinks" => false )
			      );
      $this->setCreateLinkTitle(_("Create a new Fournisseur"));      
      $bs = $this->getDialogTable();
      $bs->setTitle("Fournisseur");
      parent::setModuleName("BlueProjectPHP");			      
    }
    
    public function getTableInput($params)
    {
      $params = array_merge( array( "id" => null,
				    "cmd_id" => null,
				    "dateDoc" => strftime("%Y-%m-%d"),
				    "montantHT" => null,
				    "company_id" => null,
				    "refClient" => "",
				    "comments" => "",
				    "isValid" => 0 ),
			     $params ); 

      $rows = H();

      $readOnly = false;
      if(isset($params["id"]))
	{
	  $readOnly = true;
	}
      else
	{
	  $params["id"] = $this->getModule()->generateNewId();
	}

      $rows->row( array( _("IsValid"), createSelect( array( 0, 1 ),
						     array( "name" => "isValid",
							    "default" => $params["isValid"] ))
			 )
		  );

      $rows->row( array( _("Ref"),
			 createInputText($params["id"],
					 array( "name" => "id",
						"readonly" => $readOnly,
						"maxlength" => 12 )))
		  );

      $com = new ComCommandes($this->getModule());
      $rows->row( array( _("Cmd"),
			 createSelect( $com->getValidEntries(),
				       array( "key" => "id",
					      "name" => "cmd_id",
					      "value" => "id",
					      "default" => $params["cmd_id"],
					      "extra" => false ))
			 )
		  );
      
      $rows->row( array( _("Date"),
			 createInputDatePicker($params["dateDoc"],
					       array( "id" => "dateDocFacture",
						      "name" => "dateDoc" )))
		  );

      $rows->row( array( _("Montant HT (€)"),
			 createInputCurrency($params["montantHT"],
					     array("name" => "montantHT",
						   "id" => "FacHT",
						   "maxlength" => 12 )))
		  );

      $com = new Companies($this->getModule());
      $rows->row( array( _("Supplier"),
			 createSelect( $com->getFournisseurs(),
				       array( "key" => "company_id",
					      "name" => "company_id",
					      "value" => "company_name",
					      "default" => $params["company_id"],
					      "extra" => false ))
			 )
		  );

      $rows->row( array( _("Ref Client"),
			 createInputText($params["refClient"],
					 array( "name" => "refClient",
						"title" => "Customer's purchase order reference" )
					 )
			 )
		  );

      $rows->row( array( _("Comments"), 
			 createInputTextArea($params["comments"],
					     array( "name" => "comments", 
						    "rows" => 4, 
						    "cols" => 40 )))
		  ); 

      $table = H()->table($rows);
      $gui = H()->form($table)->withId($this->getClassName() . "EditForm");
      
      $this->setDialogParams( array( "msg" => $gui->str() ));
      $this->setEditTitle(_("Edit Fournisseur parameters"));
      $this->setCreateTitle(_("Create a new Fournisseur"));
      parent::getTableInput($params);
    }

    public function delEntrySubmission()
    {
      $this->setDeleteTitle(_("Delete a Fournisseur"));
      $this->setDialogParams(array( "msg" => H()->p(_("Are you sure to delete this Fournisseur ?"))->str()));
      parent::delEntrySubmission();
    }    

    protected function renderActions($elt, $params)
    {
      $keyparams = parse_str($params["keyparam"]);
      $filename = $this->getModule()->getFileName($elt);
      $str = "";
      if(!$elt["isValid"])
	{
	  $str .= parent::renderActions($elt, $params);
	}
      else
	{
	  $str .= parent::getViewLinks($params);
	}
      if($filename)
	{
	  // add viewing action
	  $filename = str_replace(DOCROOT, "/BlueProjectPHP/docroot", $filename);
	  
	  $str .= createLink( array("url" => $filename,
				    "nofollow" => true,
				    "value" => "<img src='/BTopLocalServer/styles/pdf-icon.png'/>" ));
	}
      return $str;
    }
}

?>