<?php

include_once("BluePHP/Utils/SimpleCodeGenerator.inc");

function CSS()
{
  return new CSS();
}

class CSS extends SimpleCodeGenerator
{
  public function __construct()
  {
    return parent::__construct();
  }

  public function body()
  {
    return $this->processAndPushNewElt("body", "");
  }

  public function sel($name)
  {
    return $this->processAndPushNewElt($name, "");
  }

  public function with($name, $value)
  {
    return $this->addAttrToLastElt($name, $value);
  }

  protected function generateStartTag($elt)
  {
    $str = "\n" . $elt["tag"] . " {";
    foreach($elt["attr"] as $k => $v)
      {
	$str .= " $k: $v; ";
      }
    return $str;
  }

  protected function generateEndTag($elt)
  {
    return "}";
  }
}

?>