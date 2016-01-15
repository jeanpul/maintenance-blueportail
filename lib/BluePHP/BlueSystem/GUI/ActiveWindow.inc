<?php

include_once("BluePHP/GUI/Frame.inc");
include_once("BluePHP/Utils/js_lang.inc");

/**
 * \class ActiveWindow
 * Base class that render the content in the
 * BlueSystem main page
 */
class ActiveWindow extends Frame
{
  private $address = null;

  public function __construct($id, $content, $basePath)
  {
    parent::__construct($id, $content, $basePath);
  }

  /**
   * Set the module address (it's not related with the id)
   * so navigation components could retrieve where this
   * window belongs in the navigation menus
   */
  protected function setAddress($a)
  {
    $this->address = $a;
  }

  /**
   * Render the window
   */
  public function render()
  {
    echo "<div id=\"" . $this->getId() . "\" class=\"ActiveWindow\">";
    $this->renderContent();
    echo "</div>";
    $this->renderAddress();
  }

  /**
   * Render the window inside content.
   * By default use then render method from the Frame 
   * i.e Use the template file.
   */
  protected function renderContent()
  {
    parent::render();
  }

  /**
   * Tell other components (such as submenu, parent window)
   * where this active window should be located into the
   * navigation menus
   */
  protected function renderAddress()
  {
    $str = "<script type='text/javascript'>";    
    $str .= js_funcall(array( "function" => "BlueSystem.declActiveWindow",
			      "options" => array( "address" => $this->address )));
    $str .= "</script>\n";
    echo $str;
  }
}

?>