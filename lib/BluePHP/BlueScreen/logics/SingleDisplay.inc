<?php

include_once("BluePHP/BlueScreen/Logics.inc");

class SingleDisplay extends Logics {

  function SingleDisplay()
  {
    parent::setParams( array( "name" => "SingleDisplay",
			      "keyname" => "id",
			      "sort" => "id" ));
  }  

  function process($params)
  {    
    $config = $this->getConfig( array( "config_id" => $params["type_data"]["config"] ) );
    $configClass = $this->getConfigClass($config);
    return $configClass->process($config);
  }

}

?>
