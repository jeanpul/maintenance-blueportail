<?php

include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/GUI/MenuEntryButton.inc");
include_once("BluePHP/BlueSystem/GUI/BSApplication.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/PanelHTML.inc");
include_once("BluePHP/BlueCountGUI/Dashboards/TabContainerInterface.inc");

class TabContainerHTML extends PanelHTML
{
  public function __construct($id, $params)
  {
    parent::__construct($id, $params);
  }

  public function getFormContent()
  {
    $params = $this->getPanel();
    $content = parent::getFormContent();
    // we cannot add dashboards in this container while it's not created
    if(isset($params["id"]))
      {
	// fab: problem with MenuEntryButton in a dialog
	// so I use an anchor
	$onClick = BS_eventRender( "BlueCountGUI.Dashboards.TabContainerInterface",
				   "id=" . $this->getId() . 
				   "&src=" . $this->getDashboard() );
	$button = "<a href=\"#\" onClick='$onClick'>" . _("Edit contained panels") . "</a>";	      
	$content->div( $button );	
      }
      return $content;
  }
}

?>