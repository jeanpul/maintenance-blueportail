<?php

include_once("BluePHP/Utils/SimpleCodeGenerator.inc");

function H()
{
  return new HTML();
}

class HTML extends SimpleCodeGenerator
{
  public function __construct()
  {
    return parent::__construct();
  }

  public function render() 
  {
    echo $this->str();
  }

  public function p($content = "")
  {
    return $this->processAndPushNewElt("p", $content);
  }

  public function form($content = "")
  {
    return $this->processAndPushNewElt("form", $content);
  }

  public function table($content = "")
  {
    return $this->processAndPushNewElt("table", $content);
  }

  public function input($content = "")
  {
    return $this->processAndPushNewElt("input", $content);
  }

  public function button($content = "")
  {
    return $this->processandPushNewElt("button", $content);
  }
   
  public function div($content = "")
  {
    return $this->processAndPushNewElt("div", $content);
  }
 
  public function h4($content = "")
  {
    return $this->processAndPushNewElt("h4", $content);
  }

  public function h3($content = "")
  {
    return $this->processAndPushNewElt("h3", $content);
  }

  public function h2($content = "")
  {
    return $this->processAndPushNewElt("h2", $content);
  }

  public function h1($content = "")
  {
    return $this->processAndPushNewElt("h1", $content);
  }

  public function ul($content = "")
  {
    return $this->processAndPushNewElt("ul", $content);
  }

  public function li($content = "")
  {
    return $this->processAndPushNewElt("li", $content);
  } 

  public function i($content = "")
  {
    return $this->processAndPushNewElt("i", $content);
  }

  public function a($content = "")
  {
    return $this->processAndPushNewElt("a", $content);
  }

  public function img($content = "")
  {
    return $this->processAndPushNewElt("img", $content);
  }

  public function tr($content = "")
  {
    return $this->processAndPushNewElt("tr", $content);
  }

  public function td($content = "")
  {
    return $this->processAndPushNewElt("td", $content);
  }

  public function th($content = "")
  {
    return $this->processAndPushNewElt("th", $content);
  }

  public function iframe($content = "")
  {
    return $this->processAndPushNewElt("iframe", $content);
  }

  public function script($content = "")
  {
    return $this->processandPushNewElt("script", $content);
  }

  public function javascript($content = "")
  {
    return $this->processandPushNewElt("script", $content)->withType("text/javascript");
  }

  public function row($elts, $isHeader = false)
  {
    $delim = $isHeader ? "th" : "td";
    $content = "";
    foreach($elts as $v)
      {
	$content .= "<$delim>" . $v . "</$delim>";
      }
    return $this->processAndPushNewElt("tr", $content);
  }

  public function with($name, $value)
  {
    return $this->addAttrToLastElt($name, $value);
  }

  public function withAction($value)
  {
    return $this->addAttrToLastElt("action", $value);
  }

  public function withId($value)
  {
    return $this->addAttrToLastElt("id", $value);
  }

  public function withClass($value)
  {
    return $this->addAttrToLastElt("class", $value);
  }

  public function withType($value)
  {
    return $this->addAttrToLastElt("type", $value);
  }

  public function withStyle($value)
  {
    return $this->addAttrToLastElt("style", $value);
  }

  public function withOnClick($value)
  {
    return $this->addAttrToLastElt("onclick", $value);
  }

  public function withOnChange($value)
  {
    return $this->addAttrToLastElt("onchange", $value);
  }

  public function withHRef($value)
  {
    return $this->addAttrToLastElt("href", $value);
  }

  public function withSrc($value)
  {
    return $this->addAttrToLastElt("src", $value);
  }

  public function withName($value)
  {
    return $this->addAttrToLastElt("name", $value);
  }

  public function withValue($value)
  {
    return $this->addAttrToLastElt("value", $value);
  }

  public function withMethod($value)
  {
    return $this->addAttrToLastElt("method", $value);
  }

  public function withEncType($value)
  {
    return $this->addAttrToLastElt("enctype", $value);
  }

  public function withMaxLength($value)
  {
    return $this->addAttrToLastElt("maxlength", $value);
  }

  public function withTarget($value)
  {
    return $this->addAttrToLastElt("target", $value);
  }

  public function withContent($content)
  {
    return $this->addContentToLastElt($content);
  }

  public function isDisabled($bool)
  {
    if($bool)
      {
	return $this->addAttrToLastElt("disabled", "disabled");
      }
    return $this;
  }

  public function isChecked($bool)
  {
    if($bool)
      {
	return $this->addAttrToLastElt("checked", "checked");
      }
    return $this;
  }

  public function setReadOnly($bool)
  {
    if($bool)
      {
	return $this->addAttrToLastElt("readonly", "readonly");
      }
    return $this;
  }

  protected function generateStartTag($elt)
  {
    $str = "<" . parent::getTag($elt);
    foreach(parent::getAttrs($elt) as $k => $v)
      {
	$str .= " $k='$v'";
      }
    $str .= ">";
    return $str;
  }

  protected function generateEndTag($elt)
  {
    return "</" . parent::getTag($elt) . ">";
  }
}

?>