<?php

include_once("BluePHP/Utils/ModuleProcessing.inc");

class ClickATell extends ModuleProcessing {

  function ClickATell()
  {
    parent::setParams( array( "name" => "ClickATell",
			      "keyname" => "id",
			      "sort" => "id" ));
  }  

  function process($api, $params)
  {    
    $str = $this->setHTTPBase($api);
    $str .= "&to=" . $params["dst"] . "&text=" . urlencode($params["content"]);
    return file_get_contents($str);
  }

  function setHTTPBase($api)
  {
    return "http://api.clickatell.com/http/sendmsg?user=" . $api["type_data"]["user"] . 
          "&password=" . $api["type_data"]["password"] . "&api_id=" . $api["type_data"]["api_id"];
  }

}

?>
