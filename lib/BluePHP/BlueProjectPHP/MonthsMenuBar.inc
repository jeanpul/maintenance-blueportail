<?php

include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/GUI/Frame.inc");

class MonthsMenuBar extends Frame
{
  private $month = null;

  public function __construct($id, $root, $params, $cb = null)
    {
      parent::__construct($id, $root, $cb);
      $this->month = $params["month"];
    }

  public function render()
  {
    $months = array();

    // generate 12 months names from mktime
    // and date function so languages will be handled
    for($i = 1; $i <= 12; $i += 1)
      {
	$months[] = array( "idx" => sprintf("%02d", $i),
			   "name" => strftime("%B", mktime(0, 0, 0, $i))
			   );
      }
    $str = createSelect($months,
			array( "id" => $this->getId(),
			       "name" => "month",
			       "default" => $this->month,
			       "key" => "idx",
			       "value" => "name",
			       "extra" => "onChange=\"changeFormValue('month', '" . $this->getId() . "')\"" )
			);
    echo $str;
  }
}

?>