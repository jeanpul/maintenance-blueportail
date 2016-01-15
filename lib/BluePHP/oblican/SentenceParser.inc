<?php

include_once("BluePHP/Utils/ModuleProcessing.inc");
include_once("BluePHP/oblican/Config.inc");

class SentenceParser extends ModuleProcessing
{
  private $sstruct = null;

  public function __construct($sharedCon = null)
  {
    parent::__construct(array(), $sharedCon);
  }
  
  protected function connect()
  {
    if(!$this->getConParams())
      {	    
	$this->setConParams( array( "type" => SESSION_DBTYPE,
				    "host" => SESSION_DBHOST,
				    "name" => SESSION_DBNAME,
				    "user" => SESSION_DBUSER,
				    "passwd" => SESSION_DBPASSWD ));
      }
    return parent::connect();
  }

  protected function getWho()
  {
    $who = "unknown";
    $upper = strtoupper($_SERVER["HTTP_USER_AGENT"]);
    if(strpos($upper, "IPAD") !== FALSE)
      {
	$who = "IPAD";
      }
    else if(strpos($upper, "ANDROID") !== FALSE)
      {
	$who = "ANDROID";
      }
    else if(strpos($upper, "LINUX") !== FALSE)
      {
	$who = "LINUX";
      }
    return $who;
  }

  protected function getWhere()
  {
    return $_SERVER["REMOTE_ADDR"];
  }

  public function parse($sentence)
  {
    $this->sstruct = array( "who" => null, 
			    "whereLocation" => null,
			    "category" => null,
			    "elts" => array() );
    
    $this->sstruct["who"] = $this->getWho();
    $this->sstruct["whereLocation"] = $this->getWhere();

    // create the normalised list of words
    $words = array_map( function($value) { return strtoupper(trim($value)); },
			explode(" ", $sentence) );

    $categoryPt = $this->findCategory($words);
    if($categoryPt)
      {

	// remove category word from the list
	// recompute indexes then find elements
	unset($words[$categoryPt[1]]);
	$words = array_values($words);

	$this->sstruct["category"] = $categoryPt[0];
	$this->sstruct["elts"] = $this->getElts($words, $categoryPt);
      }

    if($this->sstruct["category"]) 
      {
	$keywordDone = false;
	for($i = 0; !$keywordDone and $i < count($this->sstruct["elts"]); $i++)
	  {
	    $v = $this->sstruct["elts"][$i];
	    if(isset($v["keyword"]))
	      {
		$keywordDone = true;
		$c = $this->getClass($this->sstruct["category"],
				     $v["keyword"]);
		if($c)
		  {
		    include_once("BluePHP/oblican/${c}.inc");
		    $o = new $c();
		    $this->sstruct = array_merge($this->sstruct, $o->store($this->sstruct));
		  }
	      }
	  }
      }
    return $this->sstruct;
  }

  protected function findCategory($words)
  {
    $categoryPt = null;
    for($i = 0; is_null($categoryPt) && $i < count($words); $i++)
      {
	$w = $words[$i];
	if($this->isCategory($w))
	  {
	    $categoryPt = array( $w, $i );
	  }
      }
    return $categoryPt;
  }

  protected function getElts($words, $categoryPt)
  {
    $elts = array();
    while(count($words))
      {
	$word = array_shift($words);
	if($this->isKeyword($word, $categoryPt[0]))
	  {
	    $format = $this->getFormat($word);
	    if($format and count($words) > 0)
	      {
		$elts[] = array( "keyword" => $word,
				 "format" => $format,
				 "value" => array_shift($words) );
	      }
	  }
	else
	  {
	    $elts[] = array( "value" => $word );
	  }
      }
    return $elts;
  }

  protected function isKeyword($word, $category)
  {
    $res = $this->Select("* from keywords where category='$category' and name='$word'"); 
    return is_array($res) and count($res);
  }

  protected function isCategory($word)
  {
    $res = $this->Select("* from categories where name='$word'");
    return is_array($res) and count($res);
  }

  protected function getClass($category, $keyword)
  {
    $res = null;
    $data = $this->Select("* from keywords where category='$category' and name='$keyword'");
    if(is_array($data) and count($data))
      {
	$res = $data[0]["class"];	
      }
    return $res;
  }

  protected function getFormat($keyword)
  {
    $res = null;
    $data = $this->Select("* from formats where keyword='$keyword'");
    if(is_array($data) and count($data))
      {
	$res = $data[0]["name"];
      }
    return $res;
  }

}


?>