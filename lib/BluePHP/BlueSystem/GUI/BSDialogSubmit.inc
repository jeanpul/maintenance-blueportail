<?php

include_once("BluePHP/BlueSystem/GUI/BSDialogInfo.inc");

/**
 * \class BSDialogSubmit
 * A modal dialog with Apply Cancel buttons
 */
class BSDialogSubmit extends BSDialogInfo
{
  public function __construct($id, $titles, $params = null)
  {
    parent::__construct($id, $titles, $params);
    parent::setContent("BSDialogInfo");
    $this->addAttrs(array( "modal" => true,
			   "buttons" => array( "Apply" => "%applyf%",
					       "Cancel" => "%cancelf%" ) ), 
		    array( "%applyf%" => 'function() { $(this).dialog("close"); }',
			   "%cancelf%" => 'function() { $(this).dialog("close"); }' ));
  }

  public function eventSubmit($cb)
  {
    $this->eventSubmitWithName("%applyf%", $cb);
  }

  public function eventSubmitWithName($btName, $cb)
  {
    $attrs = $this->getAttrs();
    $attrs->setKeyValue($btName, 'function() { ' . $cb . ' $(this).dialog("close"); }');    
  }

  /**
   * This will replace the Apply / Cancel
   * by a Close button
   * FAB: misconception pb only needed by NumeringOverflowInterface
   * A BSModuleInterface create a BSDialogSubmit where a BSDialogInfo is required
   * maybe the action view should be used instead of edit
   */
  public function changeDialogAsInfo()
  {
    $this->addAttrs(array( "buttons" => array( "Close" => "%applyf%" ) ) );
  }

  public function addSubmitButton($btName, $btFuncName)
  {
    $attrs = $this->getAttrs();
    $this->addAttrs(array( "buttons" => array_merge( $attrs->getParamValue("buttons"), 
						     array( $btName => $btFuncName )) ),
		    array( $btFuncName => 'function() { $(this).dialog("close"); }' ));
  }
}

?>