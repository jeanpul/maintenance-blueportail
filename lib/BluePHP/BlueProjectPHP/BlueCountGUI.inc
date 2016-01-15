<?php

include_once("BluePHP/ModuleProcessing.inc");

class BlueCountGUI extends ModuleProcessing
{
  function BlueCountGUI()
  {
    parent::setParams(array( "name" => "BlueCountGUI",
			     "keyname" => "mac",
			     "sort" => "mac" ));
    parent::enquoteKey(true);
  }

  function getProducts()
  {
    return parent::Select("* from BlueCountGUI_Products where used=1 order by ref");
  }
}


?>
