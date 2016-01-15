<?php

include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/BlueSystem/GUI/BSModuleInterface.inc");
include_once("BluePHP/BlueProjectPHP/Companies.inc");

class CompaniesInterface extends BSModuleInterface
{
  public function __construct($id = "", $params = array())
  {    
    parent::__construct($id, $params);
    parent::setModule(new Companies());
    parent::setTableParams( array( "titles" => array( _("Ref"), _("Name"), _("Client?"), _("Fournisseur?") ),
				   "cols" => array( "company_client_num", "company_name", 
						    "isClient", "isFournisseur" ),
				   "cbId" => $this->getId(),
				   "cbModule" => "bp_com.companies.edit" )
			    );
    $this->setCreateLinkTitle(_("Create a new Company"));      
    $bs = $this->getDialogTable();
    $bs->setTitle("Company");
    parent::setModuleName("BlueProjectPHP");			          
  }
  
  public function getTableInput($params)
  {
    $params = array_merge( array( "company_id" => null,
				  "company_name" => null,
				  "company_client_num" => null,
				  "isClient" => 0,
				  "isFournisseur" => 0 ),
			   $params );

    $rows = H();

    if($params["company_id"] != null)
      {
	$rows->row( array( _("Id"),
			   createInputText($params["company_id"],
					   array( "name" => "company_id",
						  "readonly" => true,
						  "maxlength" => 12 )))
		    );
      }

    if($params["company_client_num"] == null)
      {
	$params["company_client_num"] = $this->getModule()->generateNewId();
      }

    $rows->row( array( _("Ref"), createInputText($params["company_client_num"],
						 array( "name" => "company_client_num", 
							"readonly" => false,
							"extra" => false )))
		);

    $rows->row( array( _("Name"), createInputText($params["company_name"],
						  array( "name" => "company_name", 
							 "readonly" => false,
							 "extra" => false )))
		);

    $rows->row( array( _("IsClient"), createSelect( array( 0, 1 ),
						    array( "name" => "isClient",
							   "default" => $params["isClient"] ))
		       )
		);

    $rows->row( array( _("IsFournisseur"), createSelect( array( 0, 1 ),
							 array( "name" => "isFournisseur",
								"default" => $params["isFournisseur"] ))
		       )
		);

    $table = H()->table($rows);
    $gui = H()->form($table)->withId($this->getClassName() . "EditForm");
    
    $this->setDialogParams( array( "msg" => $gui->str() ));
    $this->setEditTitle(_("Edit Company parameters"));
    $this->setCreateTitle(_("Create a new Company"));
    parent::getTableInput($params);
  }

  public function delEntrySubmission()
  {
    $this->setDeleteTitle(_("Delete a Company"));
    $this->setDialogParams(array( "msg" => H()->p(_("Are you sure to delete this Company ?"))->str()));
    parent::delEntrySubmission();
  }    
}

?>