<?php

include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/BlueSystem/GUI/BSModuleInterface.inc");
include_once("BluePHP/BlueProjectPHP/ComPaiements.inc");
include_once("BluePHP/BlueProjectPHP/ComFactures.inc");
include_once("BluePHP/BlueProjectPHP/ComAvoirs.inc");

class ComPaiementsInterface extends BSModuleInterface
{
  public function __construct($id = "", $params = array())
  {
    parent::__construct($id, $params);
    parent::setModule(new ComPaiements());
    parent::setTableParams( array( "titles" => array( _("Ref"), _("Client"), _("Date Payed"), _("Planned date"), _("Montant TTC (€)"), _("Montant TTC Fac (€)"), _("isPayed") ),
			    "cols" => array( "facture_id", "company_name", "datePayReal", "datePayWanted", "montantTTC", "montantTTCFac", "isPayed" ),
			    "cbId" => $this->getId(),
			    "cbModule" => "bp_com.paiements",
			    "createLinks" => false,
			    "deleteLinks" => false,
				   "viewLinks" => true )
			    );
    $this->setCreateLinkTitle(_("Create a new Paiement"));      
    $bs = $this->getDialogTable();
    $bs->setTitle("Paiement");
    parent::setModuleName("BlueProjectPHP");			          

    $table = $this->getDataTable();
    $table->setDisplayLength(25);
    $table->setSortingCols( array( array( 6, "asc" ), array( 3, "desc" ) ) );
    $table->setRowCallback( "function( nRow, aData ) {" . 
			    "var grade = \"gradeA\"; " . 
			    "if(aData[6] == \"0\") { grade = \"gradeX\"; " . 
			    "$('td:eq(2)', nRow).html('<i>not payed</i>'); " .
			    "$('td:eq(4)', nRow).html('<i>not payed</i>');} " . 
			    "$(nRow).addClass(grade);}" );
  }
    
  public function getTableInput($params)
  {
    $params = array_merge( array( "facture_id" => null,
				  "datePayWanted" => strftime("%Y-%m-%d"),
				  "datePayReal" => null,
				  "montantTTC" => null,				    
				  "charges" => 0,
				  "refBank" => "",
				  "comments" => "",
				  "isPayed" => 0 ),
			   $params ); 

    $rows = H();

    $readOnly = false;
    if(isset($params["facture_id"]))
      {
	$readOnly = true;
      }
    
    $m = new ComFactures($this->getModule());
    $facEntries = $m->getValidEntries();
    $m = new ComAvoirs($this->getModule());
    $avEntries = $m->getValidEntries();

    if($readOnly)
      {
	$rows->row( array( _("Facture"),
			   createInputText($params["facture_id"],
					   array( "name" => "facture_id",
						  "readonly" => $readOnly,
						  "maxlength" => 12 )))
		    );
      }
    else
      {
	$rows->row( array( _("Facture"), 
			   createSelect( array_merge($facEntries, $avEntries),
					 array( "name" => "facture_id",
						"key" => "id",
						"value" => "id",
						"default" => $params["facture_id"] ))
			   )
		    );
      }

    $rows->row( array( _("Planned date"),
		       createInputText($params["datePayWanted"],
				       array( "id" => "datePayWanted",
					      "name" => "datePayWanted",
					      "readonly" => true )))
		);

    $rows->row( array( _("IsPayed"), createSelect( array( 0, 1 ),
						   array( "name" => "isPayed",
							  "default" => $params["isPayed"] ))
		       )
		);

    $rows->row( array( _("Date Pay"),
		       createInputDatePicker($params["datePayReal"],
					     array( "id" => "datePayReal",
						    "name" => "datePayReal" )))
		);

    $rows->row( array( _("Montant TTC (€)"),
		       createInputCurrency($params["montantTTC"],
					   array("name" => "montantTTC",
						 "maxlength" => 12 )))
		);

    $rows->row( array( _("Charges (€)"),
		       createInputCurrency($params["charges"],
					   array("name" => "charges",
						 "maxlength" => 12 )))
		);

    $rows->row( array( _("Bank ref"),
		       createInputText($params["refBank"],
				       array( "name" => "refBank",
					      "maxlength" => 32 )))
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
    $this->setEditTitle(_("Edit Paiement parameters"));
    $this->setCreateTitle(_("Create a new Paiement"));
    parent::getTableInput($params);
  }

    public function delEntrySubmission()
    {
      $this->setDeleteTitle(_("Delete a Paiement"));
      $this->setDialogParams(array( "msg" => H()->p(_("Are you sure to delete this Paiement ?"))->str()));
      parent::delEntrySubmission();
    }    
}

?>