<?php

include_once("BluePHP/Utils/Forms.inc");

/**
 * A form page is the "squelette" of the
 * main HTML page that contains one main element.
 */
class FormPage
{
  /**
   * Page main title
   */
  var $title = "";

  /**
   * Page sub title
   */
  var $subTitle = "";

  /**
   * This is the main part of the page :
   * a graphic, a table...
   */
  var $mainPart = "";
  
  /**
   * Can contains menus, links...
   */
  var $leftPart = "";

  /**
   * Can contains the logo of the company
   */
  var $topPart = "";

  /**
   * An image to describe 
   */
  var $image = "";

  /**
   * Copyrights...
   */
  var $bottomPart = "";

  /**
   * Extra elements 
   */
  var $rightPart = "";

  /**
   * Construct an empty page
   */
  function FormPage($title, $subTitle = "")
  {
    $this->title = $title;
    $this->subTitle = $subTitle;
  }

  /*
   * HTML post declaration for XML, Keywords, StyleSheet, ...
   * this is necessary to use StyleSheet extension
   * and be referenced correctly by Google,...
   * You could pass a page title in parameter, this is then displayed on
   * the window browser application.
   * The refresh parameter is used to logout automatically user at end of
   * session time.
   */
  function pageHead($css = "css/bb.css")
  {
    // This is necessary to use the print (or echo) function
    // to avoid parsing problems with the xml start/end tags.
    echo '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">' . "\n";
    echo '<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="fr">' . "\n";
    echo "<head>\n";
    echo "<title>" . myhtmlentities($this->title) . "</title>\n";
    echo '<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />' . "\n";
    echo '<meta http-equiv="Keywords" content="Blue Eye Video, Blue Count" />' . "\n";
    echo '<meta http-equiv="Description" content="Blue Count HTML Interface."/>' . "\n";
    echo '<link rev="made" href="mailto:fabien.pelisson@blueeyevideo.com" />' . "\n";
    echo '<link rel="stylesheet" type="text/css" href="' . $css . '"/>' . "\n";
    echo "  <script type=\"text/javascript\" language=\"JavaScript\" SRC=\"" . BLUEPHPROOT . "/BluePHP/js/AnchorPosition.js\"></script>\n";
    echo "  <script type=\"text/javascript\" language=\"JavaScript\" SRC=\"" . BLUEPHPROOT . "/BluePHP/js/PopupWindow.js\"></script>\n";
    echo "  <script type=\"text/javascript\" language=\"JavaScript\" SRC=\"" . BLUEPHPROOT . "/BluePHP/js/date.js\"></script>\n";
    echo "  <script type=\"text/javascript\" language=\"JavaScript\" SRC=\"" . BLUEPHPROOT . "/BluePHP/js/CalendarPopup.js\"></script>\n";
    echo '<meta http-equiv="refresh" content="' . (SESSIONTIME + 5) . '"/>' . "\n";
    echo "</head>\n";
    echo "<body>\n";
  }

  function render()
  {
    echo '<div id="main">' . "\n";
    echo '<div id="topPart">' . "\n";
    echo "<div id=\"logo\">" . $this->topPart . "</div>\n";
    echo "<div id=\"title\"><h1>" . myhtmlentities($this->title) . "</h1></div>\n";
    echo "</div>\n";

    echo '<div id="leftPart">' . "\n";
    echo $this->leftPart;
    echo "</div>\n";

    echo '<div id="mainPart">' . "\n";
    echo $this->mainPart;
    echo "</div>\n";

    echo '<div id="rightPart">' . "\n";
    echo $this->rightPart;
    echo "</div>\n";

    echo '<div id="bottomPart">' . "\n";
    echo $this->bottomPart;
    echo "</div>\n";

    echo "</div>\n";
  }

  function pageTail()
  {
    echo "</body>\n";
    echo "</html>\n";
  }

}

/**
 * \brief Create a FormPage and uses
 * the configuration file to set a maximum of
 * parameters such as the title, the CSS, the logo,
 * the menu and the bottom part. Do not call render so
 * you can modify what you want before the rendering.
 * This could be used to get a predefined page when only
 * few parts of the page changes in your site.
 */
function createFormPageFromConfig()
{
  global $Menu;

  $page = new FormPage(TITLE);
  $page->topPart = formImage(LOGO);
  $page->mainPart = "<h1>TO BE FILLED</h1>";
  $page->leftPart = formMenu($Menu);
  $page->bottomPart = COPYRIGHT;
  $page->pageHead(CSS);

  return $page;
}

?>