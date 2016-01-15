<?php

include_once("BluePHP/Utils/ModuleProcessing.inc");
include_once("BluePHP/SmartMarket/Config.inc");

class Sites extends ModuleProcessing
{
  function Sites()
  {
    parent::setParams( array( "name" => "Sites",
			      "keyname" => "id",
			      "sort" => "name" ));
  }

  function getSiteName($params)
  {
    return parent::Select("name from Sites where id=" . $params["id"]);
  }
}

?>
