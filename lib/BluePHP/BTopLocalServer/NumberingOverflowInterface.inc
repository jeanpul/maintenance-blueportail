<?php

include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/BlueSystem/GUI/BSModuleInterface.inc");
include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/BTopLocalServer/NumberingOverflowProcessing.inc");
include_once("BluePHP/BTopLocalServer/NumberingOverflowZonesInterface.inc");

class NumberingOverflowInterface extends BSModuleInterface
{
  public function __construct($id = "", $params = array())
  {
    parent::__construct($id, $params);
    parent::setModule(new NumberingOverflowProcessing());
    $this->setCreateLinks(false);
    $this->setDeleteLinks(false);
    $this->setEditLinks(true);
    parent::setTableParams( array( "titles" => array( _("Id"), _("Location/Area") ),
				   "cols" => array( "id", "nameZ" ) 
				   ));
    $bs = $this->getDialogTable();
    $bs->setTitle(_("Numbering Overflow configuration"));
    parent::setModuleName("BTopLocalServer");
  }

  function getTableInput($params)
  {
    $rows = H()->row( array( _("Id"),
			     H()->input()->withValue($params["id"])
			     ->withType("text")->withMaxLength(8)
			     ->setReadOnly(true)->withName("id"))
		      );
    $table = H()->table($rows);
    $gui = H()->form($table)->withId($this->getClassName() . "EditForm");

    $bs = $this->getDialogEdit();
    $bs->setFrame( new NumberingOverflowZonesInterface("", 
						       array( "action" => "list", "id" => $params["id"] ))
		   );
    $bs->changeDialogAsInfo();
    $this->setDialogParams( array( "msg" => $gui->str() ));
    $this->setEditTitle(_("Edit numbering overflow parameters"));
    parent::getTableInput($params);   
  }
}

?>