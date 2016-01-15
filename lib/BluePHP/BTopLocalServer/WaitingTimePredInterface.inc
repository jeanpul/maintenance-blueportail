<?php

include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/BlueSystem/GUI/BSModuleInterface.inc");
include_once("BluePHP/BTopLocalServer/WaitingTimePredProcessing.inc");

class WaitingTimePredInterface extends BSModuleInterface
{
  public function __construct($id = "", $params = array())
  {
    parent::__construct($id, $params);
    parent::setModule(new WaitingTimePredProcessing());
    $this->setCreateLinks(false);
    $this->setDeleteLinks(false);
    $this->setEditLinks(true);
    parent::setTableParams( array( "titles" => array( _("Id"), _("Location/Area"), _("Flow smooth"), _("Min flow level"), _("Min Wait time rate") ),
				   "cols" => array( "id", "nameZ", "FlowSmooth", "MinFlowLevel", "MinWTRate" )
				   ) );
    $bs = $this->getDialogTable();
    $bs->setTitle(_("Waiting Time Prediction configuration"));
    parent::setModuleName("BTopLocalServer");
  }

  function getTableInput($params)
  {   
    $rows = H()->row( array( _("Id"),
			     H()->input()->withValue($params["id"])
			     ->withType("text")->withMaxLength(8)
			     ->setReadOnly(true)->withName("id"))
		      );

    $rows->row( array( _("Flow smooth"),
		       H()->input()->withValue($params["FlowSmooth"])
		       ->withType("text")->withMaxLength(8)
		       ->withName("FlowSmooth"))
		);
    
    $rows->row( array( _("Min flow level"),
		       H()->input()->withValue($params["MinFlowLevel"])
		       ->withType("text")->withMaxLength(8)
		       ->withName("MinFlowLevel"))
		);
    
    $rows->row( array( _("Min Wait time rate"),
		       H()->input()->withValue($params["MinWTRate"])
		       ->withType("text")->withMaxLength(8)
		       ->withName("MinWTRate"))
		);
    
    $table = H()->table($rows);
    $gui = H()->form($table)->withId($this->getClassName() . "EditForm");

    $this->setDialogParams( array( "msg" => $gui->str() ));
    $this->setEditTitle(_("Edit waiting time prediction parameters"));
    parent::getTableInput($params);
  }
}

?>