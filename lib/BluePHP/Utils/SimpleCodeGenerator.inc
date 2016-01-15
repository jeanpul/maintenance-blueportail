<?php

class SimpleCodeGenerator
{
  private $str = "";
  private $pushedElts = array();

  protected function __construct()
  {
    return $this;
  }

  public function __toString()
  {
    return $this->str();
  }

  public function str()
  {
    $this->processLastPushedElt();
    return $this->str;
  }

  protected function getTag($elt)
  {
    return $elt["tag"];
  }

  protected function getAttrs($elt)
  {
    return $elt["attr"];
  }

  protected function addAttrToLastElt($attrName, $attrValue)
  {
    if(count($this->pushedElts))
      {
	$this->pushedElts[ count($this->pushedElts) - 1 ]["attr"][$attrName] = $attrValue;
      }
    return $this;    
  }

  protected function addContentToLastElt($content)
  {
    if(count($this->pushedElts))
      {
	$this->pushedElts[ count($this->pushedElts) - 1 ]["content"] = $content;
      }
    return $this;    
  }

  protected function processAndPushNewElt($tagName, $content = "")
  {
    $this->processLastPushedElt();
    array_push( $this->pushedElts, $this->getNewElt($tagName, array(), $content) );
    return $this;
  }

  protected function getNewElt($tagName, $attr = array(), $content = "")
  {
    return array( "tag" => $tagName,
		  "attr" => $attr,
		  "content" => $content );
  }

  protected function generateStartTag($elt)
  {
    return $elt["tag"];
  }

  protected function generateEndTag($elt)
  {
    return $elt["tag"];
  }

  private function generateContent($elt)
  {
    $content = $elt["content"];
    if(is_array($content))
      {
	$str = "";
	foreach($content as $v)
	  {
	    $str .= $v;
	  }
	$content = $str;
      }
    return $content;
  }

  protected function processLastPushedElt()
  {
    $elt = array_pop($this->pushedElts);
    if(is_array($elt))
      {
	$this->str .= $this->generateStartTag($elt) . $this->generateContent($elt) . $this->generateEndTag($elt);
      }
    return $elt;
  }
}

?>