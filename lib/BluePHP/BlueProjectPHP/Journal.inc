<?php

include_once("BluePHP/ModuleProcessing.inc");


class Journal extends ModuleProcessing
{
  function Journal()
  {
    parent::setParams(array( "name" => "journal",
			     "keyname" => "journal_id",
			     "sort" => "journal_date" ));
  }

}

?>
