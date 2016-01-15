<?php

class BSPropertyFile
{
  private $filename;
  private $prop;
  private $loaded;
  
  public function __construct($file)
  {
    $this->filename = $file;
    $this->loaded = false;
  }
  
  public function loadProperties()
  {
    if(!$this->loaded)
      {
	$this->prop = $this->extractProperties($this->filename);
	$this->loaded = true;
      }
  }
  
  private function extractProperties($file)
  {
    $params = array();
    
    $handle = fopen($file, "r");
    if($handle)
      {
	while(!feof($handle))
	  {
	    $line = fgets($handle);
	    $eqpos = strpos($line, '=');
	    if($eqpos)
	      {	      
		$variable = substr($line, 0, $eqpos);
		$value = substr($line, $eqpos + 1); 
		$value = preg_replace('/\n$/', '', $value);
		
		if(preg_match('/".*"/', $value) || 
		   preg_match("/'.*'/", $value))
		  {
		    
		    eval("\$value = $value;");
		  }	      	     
		$params[$variable] = $value;
	      }
	  }
	fclose($handle);
      }  
    else
      {
	throw new Exception("file $file cannot be opened");
      }
    return $params;  
  }
  
  public function outOfDate(){
    $this->loaded = false;
  }
  
  public function getProperty($prop_name)
  {  
    return $this->prop[$prop_name];
  }
  
  public function setProperty($prop_name, $value)
  {
    $this->prop[$prop_name] = $value;
  }
  
  public function setAllProperties($prop_set)
  {
    $this->prop = $prop_set;
  }
  
  public function updateProperties($prop_set)
  {
    foreach($prop_set as $k => $v)
      {
	$this->prop[$k] = $v;
      } 
  }
  
  public function saveProperties()
  {
    $handle = fopen($this->filename, "w");
    if($handle)
      {
	foreach($this->prop as $var => $val)
	  {
	    fwrite($handle, $var . "=\"" . $val . "\"\n");
	  }
	fclose($handle);
      }
    else
      {
	throw new Exception("Cannot open file : " . $this->filename);
      }
  }
};

?>
