<?php

include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/BlueProjectPHP/BPLang.inc");
include_once("BluePHP/GUI/Frame.inc");

class SalMenuBar extends Frame
{
  private $contact_id = null;

  public function __construct($id, $root, $params, $cb = null)
    {
      parent::__construct($id, $root, $cb);
      $this->contact_id = $params["contact_id"];
    }

  public function render()
  {
    $bplang = new BPLang();
    $salNames = $bplang->getSalNames(array());    
    $str = createSelect( $salNames,
			 array( "id" => $this->getId(),
				"name" => "contact_id",
				"default" => $this->contact_id,
				"key" => "idx",
				"value" => "name",
				"extra" => "onChange=\"changeFormValue('contact_id', '" . $this->getId() . "')\"" )
			 );
    echo $str;
  }
}

?>