<?php

include_once("BluePHP/Utils/js_lang.inc");
include_once("BluePHP/BlueSystem/GUI/BSModuleInterface.inc");
include_once("BluePHP/BlueSystem/GUI/BSApplication.inc");
include_once("BluePHP/BlueProjectPHP/Config.inc");
include_once("BluePHP/BlueProjectPHP/Calendar.inc");
include_once("BluePHP/BlueProjectPHP/BPLang.inc");
include_once("BluePHP/BlueProjectPHP/Activity.inc");

class ActivityHTML extends BSModuleInterface
{
  public function __construct($id = "", $params = array())
  {
    parent::__construct($id, $params);
    parent::setModule(new Activity($params));
    parent::setTableParams( array( "titles" => array( "Date", "Day", "CIR1", "CIR2", "CIR3", "CIR4",
						      "COM1", "COM2", "COM3", "COM4", 
						      "FAM1", "FAM2", "FAM3", "FAM4" ),
				   "cols" => array( "date", "day", "cir1", "cir2", "cir3", "cir4",
						    "com1", "com2", "com3", "com4",
						    "fam1", "fam2", "fam3", "fam4" ),
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
    if(strstr($elt, "cir") !== FALSE)
      {
	$str = $this->getSelectCIR($curRow, substr($elt, -1));
      } else if(strstr($elt, "com") !== FALSE)
      {
	$str = $this->getSelectCOM($curRow, substr($elt, -1));
      }
    return $str;
  }


  private function getSelectCIR($v, $n)
  {
    $bplang = new BPLang();

    // not during the week-end use getdate
    if($bplang->isDayOff($v["date"]))
      {
	return "";
      }

    if($v["aid" . $n] == -1)
      {	
	$link = js_funcall( array( "function" => "createNewActivity", 
				   "options" => array( "action" => "new",
						       "contact_id" => $v["cid"],
						       "date" => $v["date"] )));		
	return "<a href=\"#\" onclick='$link'>new</a>";
      }
    else
      {
	$key = $v["aid" . $n] . "_cir" . $n;
	$action_id = $v["aid" . $n];
	$selVal = $v["cir" . $n];
	$catCIR = array("RD1", "RD2", "REU", "DEV", "PRE", "COM", "ADM", "FOR", "MAL", "RTT", "CP", "SS", "IMM", "NA");
	$str = "<select id=\"$key\" onChange=\"changeCIRValue({'id':'$key', action_id: '$action_id'});\">";
	foreach($catCIR as $v)
	  {
	    $selected = "";
	    if($v == $selVal)
	      {
		$selected = "selected";
	      }
	    $str .= "<option $selected>" . $v . "</option>";
	  }
	$str .= "</select>";
	return $str;
      }
  }

  function getSelectCOM($v, $n)
  {
    $bplang = new BPLang();

    // not during the week-end use getdate
    if($bplang->isDayOff($v["date"]))
      {
	return "";
      }

    if($v["aid" . $n] == -1)
      {
	return "";
      }
    else
      {
	$val = $v["com" . $n] == "" ? "<i>empty</i>" : $v["com" . $n];
	$urlEdit = js_funcall( array( "function" => "editActivity",
				      "options" => array( "action" => "edit",
							  "action_id" => $v["aid" . $n] )));
	$urlDelete = js_funcall( array( "function" => "deleteActivity", 
					"options" => array( "action_id" => $v["aid" . $n] )));		
	return "<a href=\"#\" onclick='$urlEdit'>$val</a>" . 
	  "<a href=\"#\" onclick='$urlDelete'>X</a>";
      }
  }

}

?>