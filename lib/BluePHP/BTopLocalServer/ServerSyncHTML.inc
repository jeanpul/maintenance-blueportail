<?php

include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/BlueSystem/GUI/BSApplication.inc");
include_once("BluePHP/BTopLocalServer/ServerSync.inc");
include_once("BluePHP/BlueSystem/GUI/BSDialogSubmit.inc");

/**
 * \class ServerSyncHTML
 */
class ServerSyncHTML extends BSDialogSubmit
{
  public function __construct($id, $params)
  {
    parent::__construct($id, $params);
  }

  public function render()
  {
    $module = new ServerSync();
    $params = $module->getValues();

    $formContent = H()->table( array( H()->row( array( _("Use redundant server"), 
						       createCheckBox(array( "name" => "SYNC_ENABLED",
									     "value" => "true" ),
								      $params["SYNC_ENABLED"] == "true"))),    
				      H()->row( array( _("Server adress"),
						       createInputText( $params["SYNC_SERVER" ],
									array( "name" => "SYNC_SERVER"))))
				      ));
    $gui = H()->form( $formContent )->withId("ServerSyncForm");

    $this->setTitle("Server synchronization");
    $this->setMsg($gui->str());
    $this->eventSubmit(BS_eventRenderWithForm("BTopLocalServer.ServerSyncHTML", $this->getId(), "ServerSyncForm", "submit"));
    parent::render();
  }

  public function submit()
  {
    $module = new ServerSync();
    $module->setValues($this->getParams());
  }
}

?>