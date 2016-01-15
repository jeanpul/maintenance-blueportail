<?php

include_once("BluePHP/GUI/Component.inc");
include_once("BluePHP/BlueSystem/GUI/BSDialogInfo.inc");

class HelloWorldDialog implements Component
{
  public function __construct($id, $params)
  {
  }

  public function render()
  {
    $dialog = new BSDialogInfo("hello",
			       array( "title" => "Hello world message",
				      "msg" => "Hello world !" ),
			       null);
    $dialog->render();
  }
}

?>