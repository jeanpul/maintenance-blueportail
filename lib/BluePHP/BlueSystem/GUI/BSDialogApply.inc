<?php

include_once("BluePHP/BlueSystem/GUI/BSDialogInfo.inc");

/**
 * \class BSDialogApply
 * A modal dialog with Apply Close buttons
 */
class BSDialogApply extends BSDialogInfo
{
  public function __construct($id, $titles, $params = null)
  {
    parent::__construct($id, $titles, $params);
    parent::setContent("BSDialogInfo");
    $this->addAttrs(array( "modal" => true,
			   "stack" => true,
			   "buttons" => array( array("id" => $this->getId() . "_Apply", 
						     "text" => "Apply",
						     "click"=> "%applyf%" ),
					       array( "text" => "Close",
						      "click" => "%closef%" ) 
					       ) ), 
		    array( "%applyf%" => 'function() { }',
			   "%closef%" => 'function() { $(this).dialog("destroy"); $(this).remove() }' ));
  }

  public function eventSubmit($cb)
  {
    $attrs = $this->getAttrs();
    $attrs->setKeyValue("%applyf%", 'function() { ' . $cb . ' }');
  }

}

?>