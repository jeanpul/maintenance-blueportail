<?php

include_once("BluePHP/ModuleProcessing.inc");


class VCS extends ModuleProcessing
{
  function VCS()
  {
    parent::setParams(array( "name" => "VCS",
			     "keyname" => "mac",
			     "sort" => "mac" ));
  }

  function getProducts()
  {
    return parent::Select("* from VCS_Products order by ref");
  }

  function getLots()
  {
    return parent::Select("* from VCSLots order by id desc");
  }
}

?>