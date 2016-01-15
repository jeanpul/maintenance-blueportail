<?php

include_once("BluePHP/Utils/js_lang.inc");
include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/BlueSystem/GUI/BSModuleInterface.inc");
include_once("BluePHP/BlueSystem/GUI/BSApplication.inc");
include_once("BluePHP/BlueProjectPHP/Config.inc");
include_once("BluePHP/BlueProjectPHP/HTML.inc");
include_once("BluePHP/BlueProjectPHP/Planner.inc");

class PlannerHTML extends BSModuleInterface
{
  public function __construct($id = "", $params = array())
  {
    parent::__construct($id, $params);
    parent::setModule(new Planner($params));
    parent::setTableParams( array( "titles" => array( "Date", "Day", "Q1", "Q2", "Q3", "Q4"),
				   "cols" => array("date", "day", "aid1", "aid2", "aid3", "aid4" ),
				   "cbId" => $this->getId(),
				   "createLinks" => false,
				   "editLinks" => false,
				   "deleteLinks" => false,
				   "viewLinks" => false )
			    );
    $this->setModuleName("BlueProjectPHP");

    $table = $this->getDataTable();
    $table->setDisplayLength(31);
    $table->setPaginate(false);
    $table->setInfo(false);
    $table->setFilter(false);
  }

  public function getTableListJSON($params)
  {
    $this->getModule()->setDateParams($params);
    return parent::getTableListJSON($params);
  }

  public function  getTableList($pagename = "")
  {
    $bs = $this->getDataTable();
    $bs->setContent(parent::getModuleTableList($this->getTableParams(), $pagename));
    $bs->setAjaxSource("/BluePHP/BlueSystem/CallModuleFunction.php?" . 
		       "__module=" . $this->getModuleName() . "&" . 
		       "__class=" . $this->getClassName() . "&" . 
		       "__function=getTableListJSON&" . 
		       "__output=JSON_dataTables");
    $bs->render();
  }

  protected function renderCol($elt, $value, $curRow)
  {
    $str = $value;
    if(strstr($elt, "aid") !== FALSE)
      {
	$str = $this->getSelectCategory($curRow, substr($elt, -1));
      }
    return $str;
  }

  protected function getSelectCategory($v, $n)
  {
    $bplang = new BPLang();

    // not during the week-end use getdate
    if($bplang->isDayOff($v["date"]))
      {
	return "";
      }

    $result = $this->getModule()->getAvailableActions($v);

    if(!is_array($result) or count($result) == 0)
      {
	return "empty";
      }
    
    $key = $v["date"] . "_" . $n;
    $str = "<select id=\"$key\" name='action_id' " . 
      "onChange=\"" . "changeSelectCategory('$key', '" . $v["date"] . 
      "', '$n', '" . $v["cid"] . "');\">\n";
    $result[] = array( "action_project" => 0,
		       "action_id" => -1,
		       "cir" => "ND",
		       "cat_name" => "",
		       "project_name" => "" );
    foreach($result as $o)
      {
	$selected = "";
	if($v["aid" . $n] == $o["action_id"])
	  {
	    $selected = "selected";
	  }
	$str .= "<option value='" . $o["action_id"] . "' $selected>" . 
	  $o["action_project"] . "-" . $o["cir"] . "-" . $o["cat_name"] . "/" . $o["project_name"] . 
	  "</option>\n";
      }
    $str .= "</select>\n";
    return $str;
  }

}

?>