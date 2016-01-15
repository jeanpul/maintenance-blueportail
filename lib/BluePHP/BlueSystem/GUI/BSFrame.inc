<?php

include_once("BluePHP/Utils/js_lang.inc");
include_once("BluePHP/GUI/Frame.inc");

/**
 * \class BSFrame
 * Extends Frame to provide the basic BlueSystem
 * page content window. You should inherit from this class
 * any time you want to develop a new BlueSystem page.
 * The templates location is BluePHP/BlueSystem/GUI/templates.
 * A BlueSystem Frame should belongs to a module (BlueSystem, BTopLocalServer, BQBH/Config...)
 * and provides its location inside this module that is most of the time its immediate parent.
 * For example the DateHTML BlueSystem Frame is within the BlueSystem module and its parent
 * is the MainMenu BlueSystem Frame. So when you are in the DateHTML page you have access to
 * the MainMenu from the menubar links at the top of the Frame. Moreover on the top bar Application
 * menu the BlueSystem module is automatically selected.
 * \sa BluePHPFrame
 * \todo could this class inherit BluePHPFrame ?
 * \addtogroup BlueSystem 
 * @{
 */
class BSFrame extends Frame
{
  private $root = null;
  private $params = null;
  private $links = array();
  private $system = "BlueSystem";

  /**
   * Frame construction, the template name is automatically selected
   * using the get_class($this) php function.
   * \param $id html id
   * \param $params optional parameters array
   * \param $root optional Application reference
   * \param $location optional template location
   */
  public function __construct($id, $params = null, $root = null, 
			      $location = "BluePHP/BlueSystem/GUI/templates")
  {
    parent::__construct($id, get_class($this), $location);
    $this->params = $params;
    $this->root = $root;
  }

  /**
   * Returns the Application reference
   */
  protected function getRoot()
  {
    return $this->root;
  }

  /**
   * Returns the frame parameters
   */
  protected function getParams()
  {
    return $this->params;
  }

  /**
   * Set the module name where this frame belongs.
   * The module must exist as described in the non existing BlueSystem
   * module documentation
   * \param $s module name
   */
  protected function setSystem($s)
  {
    $this->system = $s;
  }

  /**
   * Add a link to another module frame so it can be reached from
   * the menubar.
   */
  protected function pushLinksElt($elt)
  {
    array_push($this->links, $elt); 
  }

  /**
   * Render the frame menubar elements
   */
  protected function renderLinksString()
  {
    $str = "";
    if(count($this->links))
      {
	$str = "<script type='text/javascript'>";
	foreach($this->links as $elt)
	  {
	    $str .= js_funcall(array( "function" => "BlueSystem.MenuAdd",
				      "options" => array( "href" => "#address=" . $elt["address"],
							  "label" => $elt["label"] )));
	  }
	$str .= "</script>\n";
      }
    return $str;
  }

  protected function updateSystemTitle()
  {    
    $str = "<script type='text/javascript'>";    
    $str .= "$(document).ready( function() { ";
    $str .= js_funcall(array( "function" => "BlueSystem.updateSystemTitle" ) );
    $str .= "});";
    $str .= "</script>\n";
    return $str;    
  }
  
  public function render()
  {
    parent::render();
    echo $this->renderLinksString();
    //    echo $this->updateSystemTitle();
  }
}

/** @} */

?>