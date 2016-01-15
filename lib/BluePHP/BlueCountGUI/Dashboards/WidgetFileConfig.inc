<?php

include_once("BluePHP/BlueCountGUI/Config.inc");

abstract class WidgetFileConfig
{
  private $baseName = null;
  private $params = null;

  protected function __construct($name, $params)
  {
    $this->baseName = BASEPATH . "/BlueCountGUI/indicators/" . $name;
    $this->params = $params;
  } 

  public function setEntry($params)
  {
    $obj = $this->read();
    if($obj) 
      {
	$this->setParams($obj->getParams());
      }
    $this->setParams($params);
    return $this->write();
  }

  public function newEntry($params)
  {
    $this->setParams($params);
    return $this->write();
  }

  public function delEntry($params)
  {
    $this->setParams($params);
    return $this->remove();
  }
     
  public function getEntry($params)
  {
    $this->setParams($params);
    $obj = $this->read();
    if($obj) 
      {
	$this->setParams($obj->getParams());
      }
    return $this->getParams();
  }

  protected function remove()
  {
    $res = false;
    if(file_exists($this->getFileName()))
      {
	$res = unlink($this->getFileName());
      }
    return $res;
  }

  protected function write()
  {
    $str = serialize($this);    
    return file_put_contents($this->getFileName(), $str);
  }

  protected function read()
  {
    $res = null;
    if(file_exists($this->getFileName()))
      {
	$str = file_get_contents($this->getFileName());
	$res = unserialize($str);
      }
    return $res;
  }

  protected function getFileName()
  {
    return $this->baseName . "_" . $this->params["id"] . ".inc";
  }

  protected function setParams($params)
  {
    $this->params = array_merge($this->params, array_intersect_key($params, $this->params));
  }

  protected function getParams()
  {
    return $this->params;
  }

  abstract function getDefaultParams();
}

?>