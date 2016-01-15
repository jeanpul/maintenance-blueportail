<?php

include_once("BluePHP/Utils/ModuleProcessing.inc");


class Capteur extends ModuleProcessing
{
  function Capteur()
  {
    parent::setParams(array( "name" => "Capteur",
			     "keyname" => "mac",
			     "sort" => "mac" ));
    parent::enquoteKey(true);
  }

  function getProducts()
  {
    return parent::Select("* from Capteur_Products where used=1 order by ref");
  }
}

?>