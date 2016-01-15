<?php

include_once("BluePHP/Utils/ModuleProcessing.inc");

class DualDisplay extends ModuleProcessing {

  function DualDisplay()
  {
    parent::setParams( array( "name" => "DualDisplay",
			      "keyname" => "id",
			      "sort" => "id" ));
  }  

  function process($api, $params)
  {    
    return false;
  }

}

?>
