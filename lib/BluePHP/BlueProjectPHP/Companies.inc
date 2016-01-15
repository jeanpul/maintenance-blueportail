<?php

include_once("BluePHP/Utils/ModuleProcessing.inc");
include_once("BluePHP/BlueProjectPHP/Config.inc");

class Companies extends ModuleProcessing
{
  public function __construct($sharedCon = false)
  {
    parent::__construct(array( "name" => "companies",
			       "keyname" => "company_id",
			       "sort" => "company_name" ),
			$sharedCon);
  }  

  public function getClients()
  {
    return parent::Select("* from companies where isClient=1 order by company_name asc");
  }

  public function getFournisseurs()
  {
    return parent::Select("* from companies where isFournisseur=1 order by company_name asc");
  }

  public function generateNewId()
  {
    $ref = "";
    $lastEntry = $this->Select("max(company_client_num) as id from companies where company_client_num regexp '^[0-9]+$'");
    if($lastEntry)
      {
	$ref = $lastEntry[0]["id"] + 1;
      }
    return $ref;

  }
}


?>