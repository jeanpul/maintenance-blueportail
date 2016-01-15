<?php

include_once("BluePHP/Utils/JSonArray.inc");
include_once("BluePHP/BlueSystem/GUI/BSFrame.inc");

class BSDataTable extends BSFrame
{
  private $attrs = null;
  private $extra = array( "beforeCreate" => "",
			  "afterCreate" => "" );

  private $content = "";
  private $fixedHeader = false;

  public function __construct($id, $params = null)
  {
    parent::__construct($id, $params);
    $this->setId($id);
    $this->attrs = new JSonArray( array( "bProcessing" => true,
					 "bServerSide" => false,
					 "bPaginate" => true,
					 "bInfo" => true,
					 "bFilter" => true,
					 "bJQueryUI" => true,
					 "sAjaxSource"=> false ));					 
  }

  protected function getStringParams()
  {
    return $this->attrs->getString();
  }

  public function setFixedHeader($t)
  {
    $this->fixedHeader = $t;
  }

  public function isFixedHeader()
  {
    return $this->fixedHeader;
  }

  public function setServerSide($s)
  {
    $this->attrs->setParamValue("bServerSide", $s);
  }

  public function setPaginate($s)
  {
    $this->attrs->setParamValue("bPaginate", $s);
  }

  public function setInfo($s)
  {
    $this->attrs->setParamValue("bInfo", $s);
  }

  public function setFilter($s)
  {
    $this->attrs->setParamValue("bFilter", $s);
  }

  public function setAjaxSource($src)
  {
    $this->attrs->setParamValue("sAjaxSource", $src);
  }

  public function setContent($str)
  {
    $this->content = $str;
  }

  public function getContent()
  {
    return $this->content;
  }

  public function setDisplayLength($length)
  {
    $this->attrs->addParams( array( "iDisplayLength" => $length ) );
  }

  public function setSortingCols($cols)
  {
    $this->attrs->addParams( array( "aaSorting" => $cols ) );
  }
  
  public function setRowCallback($function)
  {
    $this->attrs->addParams( array( "fnRowCallback" => "%fnRowCallback%" ),
			     array( "%fnRowCallback%" => $function ) );
  }

  public function setServerParamsCallback($function)
  {
    $this->attrs->addParams( array( "fnServerParams" => "%fnServerParams%" ),
			     array( "%fnServerParams%" => $function ) );
  }

  public function setServerDataCallback($function)
  {
    $this->attrs->addParams( array( "fnServerData" => "%fnServerData%" ),
			     array( "%fnServerData%" => $function ) );
  }
}

?>