<?php

include_once("BluePHP/ModuleInterface.inc");
include_once("BluePHP/BlueProjectPHP/ActionMeeting.inc");

class ActionMeetingHTML extends ModuleInterface
{
  protected $module = null;

  function __construct($params)
  {
    parent::setName(_("ActionMeeting"));
    parent::setModule(new ActionMeeting());
    $this->setWindow($params);
  }

  function setWindow($params)
  {
    $params = array_merge( array( "action" => "list"), 
			   $params );

    $winTitle = "";
    $winContent = "";
    if($params["action"] == "list")
      {
	$winTitle = _("ActionMeeting configuration");
	$winContent = "ActionMeeting"; 
      }
    else
      {    
	$winTitle = _("ActionMeeting configuration");
	$winContent = "ActionMeetingEdit"; 
      }
    parent::setTitle($winTitle);
    parent::setContent($winContent);
    parent::setParams($params);
  }

  protected function getTableInput($params)
  {
    $str = "<table>\n";
    $str .= "</table>n";
    return $str;
  }
  
  public function getEditRef($elt, $pagename)
  {
    $pageData = array_merge( array( "query" => "" ), parse_url($pagename) );
    return $pageData["path"] . "?action=edit&" . $this->getModule()->getKeyCond($elt, "&", false) .
      "&" . $pageData["query"];
  }

}

?>
