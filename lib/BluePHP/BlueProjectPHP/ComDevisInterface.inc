<?php

include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/BlueSystem/GUI/BSModuleInterface.inc");
include_once("BluePHP/BlueProjectPHP/Contacts.inc");
include_once("BluePHP/BlueProjectPHP/ComDevis.inc");

class ComDevisInterface extends BSModuleInterface
{
    public function __construct($id = "", $params = array())
    {
      parent::__construct($id, $params);
      parent::setModule(new ComDevis());
      parent::setTableParams( array("titles" => array( _("Ref"), _("Date"), _("Author") ),
				    "cols" => array( "id", "dateDoc", "author" ),
				    "cbId" => $this->getId(),
				    "cbModule" => "bp_com.devis")
			      );
      $this->setCreateLinkTitle(_("Create a new Devis"));

      $bs = $this->getDialogTable();
      $bs->setTitle("Devis");

      parent::setModuleName("BlueProjectPHP");
    }
    
    public function getTableInput($params)
    {
      $params = array_merge( array( "id" => null,
				    "dateDoc" => strftime("%Y-%m-%d"),
				    "contact_id" => null ),
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

      $rows->row( array( _("Ref"),
			 H()->input()->withValue($params["id"])
			 ->withType("text")->withMaxLength(24)
			 ->setReadOnly($readOnly)->withName("id"))
		  );
			 
      $rows->row( array( _("Date"),
			 createInputDatePicker($params["dateDoc"],
					       array( "id" => "dateDocDevis",
						      "name" => "dateDoc" )))
		  );
      
      $con = new Contacts($this->getModule());
      $rows->row( array( _("Author"),
			 createSelect( $con->getEmployeesCommercial(),
				       array( "key" => "contact_id",
					      "name" => "contact_id",
					      "value" => "contact_first_name,contact_last_name",
					      "default" => $params["contact_id"],
					      "extra" => false ))
			 )
		  );

      $table = H()->table($rows);
      $gui = H()->form($table)->withId($this->getClassName() . "EditForm");

      $this->setDialogParams( array( "msg" => $gui->str() ));
      $this->setEditTitle(_("Edit Devis parameters"));
      $this->setCreateTitle(_("Create a new Devis"));
      parent::getTableInput($params);

    }

    public function delEntrySubmission()
    {
      $this->setDeleteTitle(_("Delete a Devis"));
      $this->setDialogParams(array( "msg" => H()->p(_("Are you sure to delete this Devis ?"))->str()));
      parent::delEntrySubmission();
    }    

    protected function renderActions($elt, $params)
    {
      $keyparams = parse_str($params["keyparam"]);
      $filename = $this->getModule()->getFileName($elt);
      $str = "";
//      if(!$elt["isValid"])
//	{
      $str .= parent::renderActions($elt, $params);
//	}
//      else
//	{
//	  $str .= parent::getViewLinks($params);
//	}
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