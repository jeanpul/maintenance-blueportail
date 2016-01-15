<?php

include_once("BluePHP/Utils/JSonArray.inc");
include_once("BluePHP/BlueSystem/GUI/BSFrame.inc");

/**
 * \class BSDialogInfo
 * Provides minimum structure to display information 
 * message to the user. 
 */
class BSDialogInfo extends BSFrame
{
  private $titles = array( "title" => "", "msg" => "");
  private $attrs = null;
  private $extra = array( "beforeCreate" => "",
			  "afterCreate" => "" );
  private $frame = null;

  /**
   * Create the dialog
   * \param $id html attribute
   * \param $titles parameters array that should have a "title" and a "msg" values
   */
  public function __construct($id, $titles = array(), $params = null)
  {
    parent::__construct($id, $params);
    $this->titles = array_merge($this->titles, $titles);
    $this->attrs = new JSonArray( array( "modal" => false,
					 "stack" => true,
					 "draggable" => true,
					 "resizable" => true,
					 "autoOpen" => true,
					 "width" => "auto",
					 "beforeClose" => "%beforeClosef%",
					 "close" => "%closef%" ),
				  array( "%closef%" => false,
					 "%beforeClosef%" => 'function() {}' ));
    $this->setDestroyAtClose(true);
  }

  public function setDestroyAtClose($b)
  {
    if($b)
      {
	$this->attrs->setKeyValue("%closef%", 
				  'function(event, ui){ $(this).dialog("destroy"); $(this).remove(); }');
      }
    else
      {
	$this->attrs->setKeyValue("%closef%", 'function() {}');
      }
  }

  public function setBeforeCloseEvent($evt)
  {
    $this->attrs->setKeyValue("%beforeClosef%", $evt);
  }

  protected function getStringParams()
  {
    return $this->attrs->getString();
  }

  /**
   * Returns the dialog title
   */
  public function getTitle()
  {
    return $this->titles["title"];
  }

  /**
   * Returns the message content
   */
  public function getMsg()
  {
    return $this->titles["msg"];
  }

  public function setTitle($title)
  {
    $this->titles["title"] = $title;
  }

  public function setMsg($msg)
  {
    $this->titles["msg"] = $msg;
  }

  public function setFrame($f)
  {
    $this->frame = $f;
  }

  public function getFrame()
  {
    return $this->frame;
  }

  public function setDialogParams($p)
  {
    $this->titles = array_merge($this->titles, $p);
  }

  protected function getAttrs()
  {
    return $this->attrs;
  }

  protected function addAttrs($a, $k = null)
  {
    $this->attrs->addParams($a, $k);
  }

  public function setAutoOpen($b)
  {
    $this->attrs->setParamValue("autoOpen", $b);
  }

  public function setModal($b)
  {
    $this->attrs->setParamValue("modal", $b);
  }

  public function setDraggable($b)
  {
    $this->attrs->setParamValue('draggable', $b);
  }

  public function setResizable($b)
  {
    $this->attrs->setParamValue('resizable', $b);
  }

  public function setWidth($b)
  {
    $this->attrs->setParamValue('width', $b);
  }

  public function setClosable($b)
  {
    $this->attrs->setParamValue('closeOnEscape', $b);
    if(!$b)
      {
	$this->addAttrs(array("open" => "%openf%"),
			array("%openf%" => 'function(event, ui) { $(".ui-dialog-titlebar-close").hide(); }'));
      }
    else
      {
	$this->addAttrs(array("open" => "%openf%"),
			array("%openf%" => 'function(event, ui) {}'));
      }
  }

  public function isAutoOpen()
  {
    return $this->attrs->getParamValue("autoOpen");
  }

  public function eventOpen()
  {
    return '$("#' . $this->getId() . '").dialog("open");';
  }

  public function eventClose()
  {
    return '$("#' . $this->getId() . '").dialog("close");';
  }

  public function getAfterCreate()
  {
    return $this->extra["afterCreate"];
  }

  public function afterCreate($cb)
  {
    $this->extra["afterCreate"] = $cb;
  }

  public function getBeforeCreate()
  {
    return $this->extra["beforeCreate"];
  }

  public function beforeCreate($cb)
  {
    $this->extra["beforeCreate"] = $cb;
  }
}

?>