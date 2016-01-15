<?php

include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/BlueSystem/GUI/BSModuleInterface.inc");
include_once("BluePHP/BTopLocalServer/NumberingOverflowZonesProcessing.inc");

/**
 * Shows the GUI for the configuration
 * of the overflow parameters for the zones
 * containing in the selected processing
 */
class NumberingOverflowZonesInterface extends BSModuleInterface
{
  public function __construct($id = "", $params = array())
  {
    parent::__construct($id, $params);
    parent::setModule(new NumberingOverflowZonesProcessing());
    $this->setCreateLinks(false);
    $this->setDeleteLinks(false);
    $this->setEditLinks(true);
    parent::setTableParams( array( "titles" => array( _("Id"), _("Cmax"), _("Nqr") ),
				   "cols" => array( "idZone", "Cmax", "Nqr" )
				   ));
    parent::setModuleName("BTopLocalServer");
  }

  function getTableInput($params)
  {
    $rows = H()->row( array( _("Id"),
			     H()->input()->withValue($params["idZone"])
			     ->withType("text")->withMaxLength(8)
			     ->setReadOnly(true)->withName("idZone"))
		      );

    $rows->row( array( _("Cmax"),
		       H()->input()->withValue($params["Cmax"])
		       ->withType("text")->withMaxLength(8)
		       ->withName("Cmax"))
		);

    $rows->row( array( _("Nqr"),
		       H()->input()->withValue($params["Nqr"])
		       ->withType("text")->withMaxLength(8)
		       ->withName("Nqr"))
		);

    $table = H()->table($rows);
    $table->input()->withType("hidden")->withName("id")->withValue($params["id"]);
    $gui = H()->form($table)->withId($this->getClassName() . "EditForm");
    
    $this->setDialogParams( array( "msg" => $gui->str() ));
    $this->setEditTitle(_("Edit numbering overflow zone parameters"));
    parent::getTableInput($params);
  }

  public function getTableList($pagename = "")
  {
    $params = $this->getParams();

    $bs = new BSDataTable($this->getClassName() . "Table");
    $bs->setContent($this->getModuleTableList($this->getTableParams(), $pagename));
    $bs->setAjaxSource("/BluePHP/BlueSystem/CallModuleFunction.php?" . 
		       "__module=" . $this->getModuleName() . "&" . 
		       "__class=" . $this->getClassName() . "&" . 
		       "__function=getTableListJSONWithId&" . 
		       "id=" . $params["id"] . "&" . 
		       "__output=JSON_dataTables");
    $bs->render();

  }

  public function getTableListJSONWithId($params)
  {
    $this->getModule()->setProcessingId($params["id"]);
    return parent::getModuleTableListJSON($this->getTableParams());
  }


}


?>