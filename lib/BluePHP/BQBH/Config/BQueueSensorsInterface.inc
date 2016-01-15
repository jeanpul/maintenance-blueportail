<?php

include_once("BluePHP/Utils/ModuleInterface.inc");
include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/BQBH/Config/BQueueSensors.inc");

class BQueueSensorsInterface extends ModuleInterface
{
  private $TableParams;

  public function __construct($id = "", $params = array())
    {
      parent::__construct($id, $params);
      parent::setModule(new BQueueSensors());

      $this->TableParams = array("ctitle" => null,
				 "titles" => array( _("Id"), _("View"), _("IP"), _("Status") ),
				 "cols" => array( "id", "view", "device", "status" ),
				 "cbId" => $this->getId(),
				 "cbModule" => "BQBH/Config",
				 "deleteLinks" => false,
				 "createLinks" => false );
    }
  
  function getTableInput($params)
  {
    $str = "<table>\n";
    $str .= "</table>\n";
    return $str;
  }

  protected function getTableList($pagename = "")
  {
    $str = "<script type='text/javascript'>\n";
    $str .= "BlueSystem.ModuleCreateTable({'id':'" . $this->TableParams["cbId"] . "'" . 
      ",'module':'" . $this->TableParams["cbModule"] . "'});\n";
    $str .= "</script>\n";
    return parent::getModuleTableList($this->TableParams, $pagename) . $str;
  }

  public function getTableListJSON($params = null)
  {
    return parent::getModuleTableListJSON($this->TableParams);
  }

  protected function renderCol($colname, $elt)
  {
    if($colname == "view")
      {
	return "<img height='40' src='$elt'></img>";
      }
    else if($colname == "device")
      {
	// separate the device type from the device information
	list($devType, $devInfo) = explode(":", $elt, 2);
	if($devType == "ip")
	  {
	    // retrieve the host name (or IP adress) from the URL
	    $urlElts = parse_url($devInfo);
	    return $urlElts["host"];
	  }
	else
	  {
	    return _("Not an IP camera");
	  }
      }
    else
      {
	return parent::renderCol($colname, $elt);
      }
  }

  protected function renderActions($elt, $params)
  {
    $str = parent::renderActions($elt, $params);
    return $str;
  }
}

?>