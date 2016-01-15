<?php

include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/BlueSystem/GUI/BSModuleInterface.inc");
include_once("BluePHP/BlueProjectPHP/Projects.inc");
include_once("BluePHP/BlueProjectPHP/Products.inc");
include_once("BluePHP/BlueProjectPHP/ComCommandes.inc");
include_once("BluePHP/BlueProjectPHP/Livraison.inc");

class LivraisonInterface extends BSModuleInterface
{
  function __construct($id = "", $params = array())
  {
    parent::__construct($id, $params);
    parent::setModule(new Livraison());
    parent::setTableParams( array( "titles" => array( _("Ref"), _("Date"), _("Commande"), _("Project"), _("Product"), _("isValid") ),
				   "cols" => array( "ref", "dateSend", "commande", "project_name", "product_name" , "isValid" ),
				   "cbId" => $this->getId(),
				   "cbModule" => "bp_com.livraisons"
				   )
			    );
      $this->setCreateLinkTitle(_("Create a new Livraison"));      
      $bs = $this->getDialogTable();
      $bs->setTitle("Livraison");
      parent::setModuleName("BlueProjectPHP");			      
  }
  
  public function getTableInput($params)
  {
    $params = array_merge( array( "livraison_id" => null,
				  "ref" => null,
				  "commande" => null,
				  "project_id" => 0,
				  "comments" => null,
				  "product_id" => null,
				  "dateSend" => strftime("%Y-%m-%d"),
				  "isValid" => 0 ),
			   $params );
    
    $rows = H();

    $readOnly = false;
    if(isset($params["ref"]))
      {
	$readOnly = true;
      }
    else
      {
	$params["ref"] = $this->getModule()->generateNewId();
      }

    $rows->row( array( _("IsValid"), createSelect( array( 0, 1 ),
						   array( "name" => "isValid",
							  "default" => $params["isValid"] ))
		       )
		);

    $rows->row( array( _("Ref"),
		       createInputText($params["ref"],
				       array( "name" => "ref",
					      "maxlength" => 32 )))
		);

    $pmodule = new Products();
    $rows->row( array( _("Products"), createSelect($pmodule->getEntries(),
						   array( "value" => "product_name", 
							  "name" => $pmodule->getKey(), 
							  "key" => $pmodule->getKey(), 
							  "default" => $params[$pmodule->getKey()], 
							  "extra" => false )))
		);
  
    $cmodule = new ComCommandes($this->getModule());
    $rows->row( array( _("Commande"), createSelect($cmodule->getEntries(),
						   array( "value" => "id",
							  "name" => "commande",
							  "key" => "id",
							  "default" => $params["commande"],
							  "extra" => false )))
		);
    
    $rows->row( array( _("Date"),
		       createInputDatePicker($params["dateSend"],
					     array( "id" => "dateSendLivraison",
						    "name" => "dateSend" )))
		);
    
    $rows->row( array( _("Comments"), createInputTextArea($params["comments"], 
							  array( "name" => "comments",
								 "rows" => 4, 
								 "cols" => 40, 
								 "extra" => false )))
		);
    
    $table = H()->table($rows);
    $table->input()->withType("hidden")->withName("project_id")->withValue($params["project_id"]);

    if(!is_null($params["livraison_id"]))
      {
	$table->input()->withType("hidden")->withName("livraison_id")->withValue($params["livraison_id"]);
      }

    $gui = H()->form($table)->withId($this->getClassName() . "EditForm");
    
    $this->setDialogParams( array( "msg" => $gui->str() ));
    $this->setEditTitle(_("Edit Livraison parameters"));
    $this->setCreateTitle(_("Create a new Livraison"));
    parent::getTableInput($params);
  }

  public function delEntrySubmission()
  {
    $this->setDeleteTitle(_("Delete a Livraison"));
    $this->setDialogParams(array( "msg" => H()->p(_("Are you sure to delete this Livraison ?"))->str()));
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
