<?php

include_once("BluePHP/Utils/js_lang.inc");
include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/BlueSystem/GUI/BSModuleInterface.inc");
include_once("BluePHP/BlueProjectPHP/Companies.inc");
include_once("BluePHP/BlueProjectPHP/ComCommandes.inc");
include_once("BluePHP/BlueProjectPHP/ComFactures.inc");

class ComFacturesInterface extends BSModuleInterface
{
    public function __construct($id = "", $params = array())
    {
      parent::__construct($id, $params);
      parent::setModule(new ComFactures());
      parent::setTableParams( array( "titles" => array( _("Ref"), _("Cmd"), _("Date"), _("Montant HT (€)"), _("Client"), _("isValid") ),
				     "cols" => array( "id", "cmd_id", "dateDoc", "montantHT", "clientName", "isValid" ),
				     "cbId" => $this->getId(),
				     "cbModule" => "bp_com.factures",
				     "viewLinks" => false )
			      );
      $this->setCreateLinkTitle(_("Create a new Facture"));      
      $bs = $this->getDialogTable();
      $bs->setTitle("Facture");
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
				    "cmd_id" => null,
				    "dateDoc" => strftime("%Y-%m-%d"),
				    "montantHT" => null,
				    "montantTTC" => null,
				    "company_id" => null,
				    "datePay" => null,
				    "project_id" => null,
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
      
      $button = "<input type='button' value='19.6%' onClick=\"BlueProjectPHP.ApplyVAT({'src':'FacHT','dst':'FacTTC','vat':19.6});\" />";
      
      $rows->row( array( _("Montant HT (€)"),
			 createInputCurrency($params["montantHT"],
					     array("name" => "montantHT",
						   "id" => "FacHT",
						   "maxlength" => 12 )) .
			 $button )
		  );
				     
      $rows->row( array( _("Montant TTC (€)"),
			 createInputCurrency($params["montantTTC"],
					     array("name" => "montantTTC",
						   "id" => "FacTTC",
						   "maxlength" => 12 )))
		  );

      $rows->row( array( _("ToPay"),
			 createInputDatePicker($params["datePay"],
					       array( "id" => "datePayFacture",
						      "name" => "datePay" )))
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
      $this->setEditTitle(_("Edit Facture parameters"));
      $this->setCreateTitle(_("Create a new Facture"));
      parent::getTableInput($params);
    }

    public function delEntrySubmission()
    {
      $this->setDeleteTitle(_("Delete a Facture"));
      $this->setDialogParams(array( "msg" => H()->p(_("Are you sure to delete this Facture ?"))->str()));
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