<?php

include_once("BluePHP/GUI/Anchor.inc");
include_once("BluePHP/BlueProjectPHP/BPLang.inc");
include_once("BluePHP/JQUERY/MenuBar.inc");

class CatMenuBar extends MenuBar
{
  public function __construct($id, $root, $params, $cb = null)
    {
      parent::__construct($id, $root, $cb);
      $this->add_elts($params);
    }

  private function add_elts($params)
  {
    $catNames = array( "RD1", "RD2", "DEV", "ADM", "COM", "PRE", 
		       "REU", "FOR", "MAL", "CP", "RTT", "SS", "IMM", "NA" );

    foreach($catNames as $cat)
      {
	$params["cir"] = $cat;
	$this->add(new Anchor($params["pageName"], $cat, $params));
      }
  }
}

?>

