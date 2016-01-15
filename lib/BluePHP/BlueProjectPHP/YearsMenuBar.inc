<?php

include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/GUI/Frame.inc");

class YearsMenuBar extends Frame
{
  private $year = null;

  public function __construct($id, $root, $params, $cb = null)
    {
      parent::__construct($id, $root, $cb);
      $this->year = $params["year"];
    }

  public function render()
  {
    $years = array( "2009", "2010", "2011", "2012", "2013", "2014" );
    $str = createSelect($years,
			array( "id" => $this->getId(),
			       "name" => "year",
			       "default" => $this->year,
			       "extra" => "onChange=\"changeFormValue('year', '" . $this->getId() . "')\"" )
			);
    echo $str;
  }
}

?>