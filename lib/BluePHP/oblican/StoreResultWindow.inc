<?php

include_once("BluePHP/oblican/ObliFrame.inc");
include_once("BluePHP/oblican/SentenceParser.inc");

class StoreResultWindow extends ObliFrame
{
  private $query = null;
  private $parser = null;

  public function __construct($query = null) 
  {
    parent::__construct();
    $this->query = $query;
    $this->parser = new SentenceParser();
  }

  protected function getQuery()
  {
    return $this->query;
  }

  protected function getStructure()
  {
    return $this->parser->parse($this->getQuery());
  }
}

?>
