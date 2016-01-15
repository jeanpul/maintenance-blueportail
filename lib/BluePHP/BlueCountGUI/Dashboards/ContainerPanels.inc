<?php

include_once("BluePHP/BlueCountGUI/Config.inc");
include_once("BluePHP/BlueCountGUI/BCGUI_ModuleProcessing.inc");

class ContainerPanels extends BCGUI_ModuleProcessing
{
  public function __construct($sharedCon = null)
  {
    parent::__construct(array( "name" => "ContainerPanels",
			       "keyname" => "id,idd,position",
			       "sort" => "position" ),
			$sharedCon);
  }

  public function getEntriesForPanel($params)
  {
    $str = "ContainerPanels.id as id, ContainerPanels.idd as idd, ContainerPanels.position as position," . 
      "ContainerPanels.label as label, Dashboards.label as dlabel from ContainerPanels, Dashboards where ContainerPanels.id=" . $params["id"] . 
      " and ContainerPanels.idd=Dashboards.id order by ContainerPanels.position asc";
    return parent::Select($str);
  }

  public function getNextPos($params)
  {
    $maxPos = 0;
    $str = "max(position) as p from ContainerPanels where id=" . $params["id"];
    $res = parent::Select($str);
    if($res[0]["p"] != null)
      {
	$maxPos = $res[0]["p"] + 1;
      }
    return $maxPos;
  }
}

?>