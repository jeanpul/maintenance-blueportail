<?php

include_once("BluePHP/Utils/Input.inc");

/**
 * \class ComConfiguration
 *
 */
class ComConfiguration 
{
  private $params = null;

  public function __construct()
  {
  }

  public function setEntry($params)
  {
    return $this->setComParams($params);
  }

  public function getEntry($params)
  {
    return $this->getComParams();
  }

  public function setComParams($params)
  {
    $params = array_merge($this->getComParams(),
			  $params);
    $str = "source /etc/BEV/scripts/Communication/CollectValues.inc\n" . 
      $this->print_params($params) . 
      "CollectValues_setValues";
    exec($str);
    $this->params = $params;
    return $this->params;
  }

  public function getComParams()
  {
    $str = "source /etc/BEV/scripts/Communication/CollectValues.inc\n" .     
      "CollectValues_getValues\n" . 
      "CollectValues_exportValues /dev/stdout";
    $output = array();
    exec($str, $output);
    $this->params = parseSimplePairValues($output);
    return $this->params;
  }

  public function testComParams()
  {
    $str = "source /etc/BEV/scripts/Communication/BlueHTTP.inc\n" . 
      "BlueHTTP_testQuery";
    $output = array();
    $res = 0;    
    exec($str, $output, $res);
    return $res;
  }

  public function getComURL()
  {
    return "http://" . $this->params["COLLECTVALUES_DEST_SERVER"] . ":" .  
      $this->params["COLLECTVALUES_DEST_PORT"] . $this->params["COLLECTVALUES_DEST_SCRIPT"];
  }

  private function print_params($params)
  {
    $str = "";
    foreach($params as $k => $v)
      {
	if(substr($k, 0, 13) == "COLLECTVALUES")
	  {
	    $str .= $k . "=" . $v . "\n";
	  }
      }
    return $str;
  }
}

?>