<?php

/**
 * \class FormPageStandard
 * \brief A page with predefined parts
 *
 * Should met the requirements for most of
 * our applications. Example of a layout that can be displayed
 * with this class :\n
 * ********************************\n
 * LOGO TITLE\n
 * ********************************\n
 * TOP MENU\n
 * ********************************\n
 * MAIN MENU\n
 * ********************************\n
 *
 * A SET OF BOXES\n
 *
 * ********************************\n
 * BOTTOM MENU\n
 * ********************************\n
 *
 * Where 
 * - A Menu is an array of entry
 * where you can either put a link or
 * simply displays some useful informations
 * such as the current date, the user name...
 * - A logo is simply a string or a formImage
 * - A title is a string
 * - The main part could be any HTML
 */

class FormPageStandard
{
  var $symbols = array("PREMAINMENU" => "<div id=\"MainMenu\">\n",
		       "POSTMAINMENU" => "</div>\n");

  /**
   * \brief The default top menu.
   * 
   * This is an assoc array in the form
   * array( title => link, ...) where
   * link is optional (empty = no link).
   */
  var $topMenu;

  /**
   * \brief The file name of the logo image
   */
  var $logo;

  /**
   * \brief The main title of the page
   */
  var $title;

  /**
   * \brief The main menu.
   * 
   * For the format \see FormPageStandard::$topMenu
   */
  var $mainMenu;

  /**
   * \brief The page content
   */
  var $boxes = array();

  /**
   * \brief The page content
   */
  var $mainPart = "";

  /**
   * \brief The bottom menu.
   *
   * For the format \see FormPageStandard::$topMenu
   */
  var $bottomMenu;

  /**
   * \brief Create the page with empty parts.
   * 
   * To set the differents parts use
   * the modifiers
   */
  function FormPageStandard()
    {
    }

  /**
   * \brief Set the top menu.
   */
  function setTopMenu($menu)
    {
      $this->topMenu = $menu;
    }

  /**
   * \brief Set the logo file name.
   */
  function setLogo($logo)
    {
      $this->logo = $logo;
    }

  /**
   * \brief Set the page title.
   */
  function setTitle($title)
    {
      $this->title = $title;
    }

  /**
   * \brief Set the main menu.
   */
  function setMainMenu($menu)
    {
      $this->mainMenu = $menu;
    }

  /**
   * \brief Set the bottom menu.
   */
  function setBottomMenu($menu)
    {
      if(!is_array($menu))
	{
	  exit("FormPageStandard::setBottomMenu " . "menu is not an array");
	}     
      $this->bottomMenu = $menu;
    }

  /**
   * \brief Set the contents directly
   * do not use boxes
   */
  function setMainPart($str)
    {
      $this->mainPart = $str;
    }

  /**
   * \brief Add a box into the main part
   * \param $box the string content of the box
   * \param $id id added to the div container (must be unique)
   */ 
  function addBox($id, $box)
    {
      $this->boxes[] = array( "id" => $id,
			      "content" => $box );
    }

  /**
   * \brief Set symbols
   */
  function setSymbols($symbol, $value)
    {
      $this->symbols[$symbol] = $value;
    }

  /**
   * Returns the text to
   * display
   */
  function toString()
    {
      $str = '<div id="main">' . "\n";
      $str .= '<div id="Logo"><img src="styles/' . $this->logo . '" alt="Logo"/></div>' . "\n";
      $str .= '<div id="TopMenu" class="Bars">' . $this->topMenu . "</div>\n";
      $str .= $this->symbols["PREMAINMENU"] . $this->mainMenu . $this->symbols["POSTMAINMENU"];
      $str .= '<div class="MainPart">';
      if($this->mainPart)
	{
	  $str .= $this->mainPart;
	}
      else
	{
	  foreach($this->boxes as $v)
	    {
	      $str .= '<div id="' . $v["id"] . '" class="Boxes">' . $v["content"] . "</div>\n";
	    }
	}
      $str .= "</div>\n";
      $str .= '<div id="BottomMenu" class="Bars">' . formMenu($this->bottomMenu, "BottomMenu") . "</div>\n";
      $str .= "</div>\n";
      return $str;
    }
}

?>

