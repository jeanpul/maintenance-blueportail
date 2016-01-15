<?php

include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/BlueSystem/GUI/BSModuleInterface.inc");
include_once("BluePHP/BlueProjectPHP/Companies.inc");
include_once("BluePHP/BlueProjectPHP/Projects.inc");
include_once("BluePHP/BlueProjectPHP/ComCommandes.inc");

class ComCommandesInterface extends BSModuleInterface
{
    public function __construct($id = "", $params = array())
    {
      parent::__construct($id, $params);
      parent::setModule(new ComCommandes());
      parent::setTableParams( array("titles" => array( _("Ref"), _("Date"), _("Montant HT (€)"), _("Client"), _("Projet Wiki"), _("isValid") ),
				    "cols" => array( "id", "dateDoc", "montantHT", "clientName", "project_id", "isValid" ),
				    "cbId" => $this->getId(),
				    "cbModule" => "bp_com.commandes")
			      );
      $this->setCreateLinkTitle(_("Create a new Commande"));
      
      $bs = $this->getDialogTable();
      $bs->setTitle("Commandes");
      
      parent::setModuleName("BlueProjectPHP");

      $table = $this->getDataTable();
      $table->setDisplayLength(25);
      $table->setSortingCols( array( array( 0, "desc" ) ) );
      $table->setRowCallback( "function( nRow, aData ) {" . 
			      "var grade = aData[5] == \"1\" ? \"gradeA\" : \"gradeX\"; " .
			      "$(nRow).addClass(grade);}" );
    }
    
    public function getTableInput($params)
    {
      $params = array_merge( array( "id" => null,
				    "dateDoc" => strftime("%Y-%m-%d"),
				    "montantHT" => null,
				    "company_id" => null,
				    "refClient" => null,
				    "isExport" => 0,
				    "comments" => "",
				    "project_id" => null,
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
      
      $rows->row( array( _("Date"),
			 createInputDatePicker($params["dateDoc"],
					       array( "id" => "dateDocCommande",
						      "name" => "dateDoc" )))
		  );
      
      $rows->row( array( _("Montant"),
			 createInputCurrency($params["montantHT"],
					     array("name" => "montantHT",
						   "maxlength" => 12,
						   "allowNegative" => true )))
		  );

      $com = new Companies($this->getModule());
      $rows->row( array( _("Client"),
			 createSelect( $com->getClients(),
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

      $rows->row( array( _("Export?"), createSelect( array( 0, 1 ),
						     array( "name" => "isExport",
							    "default" => $params["isExport"] ))
			 )
		  );

      $p = new Projects();
      $rows->row( array( _("Projet"), createSelect( $p->getSuivis(),
						    array( "value" => "project_name",
							   "name" => "project_id",
							   "key" => "id",
							   "default" => $params["project_id"] ))
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
      $this->setEditTitle(_("Edit Commande parameters"));
      $this->setCreateTitle(_("Create a new Commande"));
      parent::getTableInput($params);
    }

    public function delEntrySubmission()
    {
      $this->setDeleteTitle(_("Delete a Commande"));
      $this->setDialogParams(array( "msg" => H()->p(_("Are you sure to delete this Commande ?"))->str()));
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