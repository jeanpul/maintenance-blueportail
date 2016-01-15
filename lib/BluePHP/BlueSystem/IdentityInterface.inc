<?php

include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/BlueSystem/GUI/BSApplication.inc");
include_once("BluePHP/BlueSystem/BlueSystem.inc");
include_once("BluePHP/BlueSystem/GUI/BSDialogSubmit.inc");

/**
 * \class IdentityInterface
 */
class IdentityInterface extends BSDialogSubmit
{
  public function __construct($id, $params)
  {
    parent::__construct($id, array(), $params);
    parent::setContent("BSDialogSubmit");
  }

  public function render()
  {
    $params = $this->getParams();
    $module = new Bluesystem();
    $params = array_merge( $params, array( "clientId" => $module->getClientId() ));

    $gui = H()->h2("Set the server identity name");
    $gui->p("The identity name (clientId) must be unique and is used as a reference for the maintenance process.")
      ->withStyle("width: 500px");

    $rows = H()->row( array( "ClientId",
			     H()->input()->withType("text")->withName("clientId")
			     ->withValue($params["clientId"])
			     ) );
    
    $gui->form( H()->table( $rows ) )->withId("IdentityForm");
    
    $this->setTitle("Identity configuration");
    $this->setMsg($gui->str());
    $this->eventSubmit(BS_eventRenderWithForm("BlueSystem.IdentityInterface", $this->getId(), "IdentityForm", "submit"));
    parent::render();
  }

  public function submit()
  {    
    $module = new BlueSystem();
    $params = $this->getParams();
    $module->setClientId($params["clientId"]);
  }

}

?>