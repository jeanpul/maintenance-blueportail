<?php

include_once("BluePHP/BlueProjectPHP/Config.inc");
include_once("BluePHP/Utils/ModuleProcessing.inc");


class Products extends ModuleProcessing
{
  public function __construct($sharedCon = false)
  {
    parent::__construct(array( "name" => "products",
				"keyname" => "product_id",
			       "sort" => "product_name" ),
			$sharedCon);
  }

}

?>

