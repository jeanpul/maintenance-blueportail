<?php

include_once("BluePHP/Utils/ModuleProcessing.inc");
include_once("BluePHP/BlueProjectPHP/Config.inc");

class BQueue extends ModuleProcessing
{
  function BQueue()
  {
    parent::setParams(array( "name" => "BQueue",
			     "keyname" => "mac",
			     "sort" => "mac" ));
    parent::enquoteKey(true);
  }

  function getProducts()
  {
    return parent::Select("* from BQueue_Products where used=1 order by ref");
  }

  function getServersFromProject($params)
  {
    return parent::Select("* from BQueue where project_id=" . $params["project_id"]);
  }

  function getChannelsFromServer($params)
  {
    return parent::Select("Capteur_mac as Capteur_mac, BC.Id as Id, BC.clientId as clientId, " .
			  "BC.Server_id as Server_id, BC.Label as Label, " .
			  "Capteur.product_ref as Capteur_product, Capteur.IP as Capteur_IP " .
			  "from BQueue_Channel as BC, Capteur where Capteur.mac=BC.Capteur_mac and clientId=\"" . $params["clientId"] . "\"");
  }
}

?>