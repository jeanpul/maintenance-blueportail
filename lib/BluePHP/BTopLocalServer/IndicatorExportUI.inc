<?php

include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/BlueSystem/GUI/BSModuleInterface.inc");
include_once("BluePHP/BTopLocalServer/IndicatorExport.inc");

class IndicatorExportUI extends BSModuleInterface
{
  public function __construct($id = "", $params = array())
  {
    parent::__construct($id, $params);
    parent::setModule(new IndicatorExport());
    parent::setTableParams( array("titles" => array( _("Indicator"), _("Id"), _("Module"), _("doExport") ),
				  "cols" => array( "indicator", "id", "module", "doExport" )
				  ));
    $this->setCreateLinkTitle(_("Export an indicator"));

    $bs = $this->getDialogTable();
    $bs->setTitle(_("Indicators export configuration"));

    parent::setModuleName("BTopLocalServer");
  }

  public function getTableInput($params)
  {
    $params = array_merge(array("doExport" => 1,				
				"id" => null,
				"indicator" => "FlowCountingProcessing", 
				"module" => "getLastMinuteValues" ), $params);
    
    $rows = H();

    if(!is_null($params["id"]))
      {
	$rows->row( array( _("Indicator"),
			   H()->input()->withValue($params["indicator"])
			   ->withType("text")->withMaxLength(64)
			   ->setReadOnly(true)->withName("indicator"))
		    );
	$rows->row( array( _("Id"),
			   H()->input()->withValue($params["id"])
			   ->withType("text")->withMaxLength(8)
			   ->setReadOnly(true)->withName("id"))
		    );
	$rows->row( array( _("Module"),
			   H()->input()->withValue($params["module"])
			   ->withType("text")->withMaxLength(128)
			   ->setReadOnly(true)->withName("module"))
		    );

      }
    else
      {
	$rows->row( array( _("Indicator"),
			   createSelect( $this->getModule()->getIndicatorsName(),
					 array ("name" => "indicator",
						"default" => $params["indicator"],
						"extra" => "onChange=\"BTopLocalServer.ModuleChangeIndicator('IndicatorExportUI','indicator')\"" )))
		    );

	$rows->row( array( _("Id"),
			   createSelect( $this->getModule()->getProcessingIds( array( "indicator" => $params["indicator"] ) ),
					 array ("name" => "id",
						"key" => "id",
						"value" => "id" )))
		    );			   

	$rows->row( array( _("Module"),
			   createSelect( $this->getModule()->getModulesName(),
					 array ("name" => "module",
						"default" => $params["module"] )))
		    );

      }

    $rows->row( array( _("Export"),
		       H()->input()->withValue(1)
		       ->withType("checkbox")
		       ->withName("doExport")
		       ->isChecked($params["doExport"] == 1)
		       )
		);

    $table = H()->table($rows);

    $gui = H()->form($table)->withId($this->getClassName() . "EditForm");

    $this->setDialogParams( array( "msg" => $gui->str() ));
    $this->setEditTitle(_("Edit Indicator export parameters"));
    $this->setCreateTitle(_("Export an indicator"));
    parent::getTableInput($params);
  }

  public function delEntrySubmission()
  {
//    $params = $this->getParams();
//    $loc = $this->getModule()->getEntry( array( "id" => $params["id"] ) );
//    $locName = $loc["name"];
    $this->setDeleteTitle(_("Remove an indicator export"));
    $this->setDialogParams(array( "msg" => H()->p(_("Are you sure to remove this export ?"))->str()));
    parent::delEntrySubmission();
  }
}

?>