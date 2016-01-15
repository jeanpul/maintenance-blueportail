<?php

class JSonArray
{
  private $params = array();
  private $keys = null;

  public function __construct($params = array(), $keys = null)
  {
    $this->setParams($params, $keys);
  }

  public function setParams($params, $keys = null)
  {
    $this->params = $params;
    $this->keys = $keys;
  }

  public function addParams($params, $keys = null)
  {
    $this->params = array_merge($this->params, $params);
    if(!$this->keys)
      {
	$this->keys = $keys;
      }
    else if($keys)
      {
	$this->keys = array_merge($this->keys, $keys);
      }
  }

  public function setParamValue($k, $v)
  {
    $this->params[$k] = $v;
  }

  public function getParamValue($k)
  {
    return $this->params[$k];
  }

  public function setKeyValue($k, $v)
  {
    $this->keys[$k] = $v;
  }

  public function getString()
  {
    $json = json_encode($this->params);
    if($this->keys) 
      {
	foreach($this->keys as $k => $v)
	  {
	    $json = str_replace('"' . $k . '"', $v, $json);
	  }
      }
    return $json;
  }
}

?>