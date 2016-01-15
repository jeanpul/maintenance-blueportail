<?php

include_once("BluePHP/Utils/ModuleProcessing.inc");

class Status extends ModuleProcessing
{
  function Status()
  {
    parent::setParams( array( "name" => "status",
			      "keyname" => "counters_id,file" ));    
  }

  function getStatusFromCounter($params)
  {
    return parent::Select("* from status where counters_id=" . $params["counters_id"]);
  }

}

?>

