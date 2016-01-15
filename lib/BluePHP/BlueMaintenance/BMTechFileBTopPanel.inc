<?php

include_once("BluePHP/BlueMaintenance/BMFrame.inc");

class BMTechFileBTopPanel extends BMFrame
{
  private $data = null;

  public function __construct($id, $params, $data = null)
  {
    parent::__construct($id, $params);
    if(!is_null($data))
      {
	$this->data = $data;
      }
  }

  public function getStatus()
  {
    return $this->data;
  }
}

?>