<?php

include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/BlueSystem/GUI/BSApplication.inc");
include_once("BluePHP/BlueSystem/BlueSystem.inc");
include_once("BluePHP/BlueSystem/GUI/BSDialogInfo.inc");

/**
 * \class SystemInfoInterface
 */
class SystemInfoInterface extends BSDialogInfo
{
  public function __construct($id, $params)
  {
    parent::__construct($id, array(), $params);
    parent::setContent("BSDialogInfo");
  }

  public function render()
  {
    $module = new Bluesystem();
    $reference = $module->getReference();
    $infoList = H()->li("version: " . $reference["BLUESYSTEM_APPLI"] . " " 
			. $reference["BLUESYSTEM_VERSION"])
      ->li("rev: " . $reference["BLUESYSTEM_REVISION"])
      ->li("mac address: " . $reference["MAC"])
      ->li("Client Id: " . $module->getClientId())
      ->li("support@blueeyevideo.com +33 981 993 804");
    $info = H()->ul( $infoList )->withId("reference");
    //    $info = H()->ul( $infoList )->withId("reference")->ul()->withId("system");
    $this->setBeforeCloseEvent("function() { BlueSystem.stopSystemInfoInterface(); }");
    $this->setTitle("System Status");
    $this->setMsg($info->str());
    parent::render();
  }
}

?>