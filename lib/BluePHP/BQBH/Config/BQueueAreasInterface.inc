<?php

include_once("BluePHP/Utils/ModuleInterface.inc");
include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/BQBH/Config/BQueueAreas.inc");

class BQueueAreasInterface extends ModuleInterface
{
  private $TableParams;

  public function __construct($id = "", $params = array())
    {
      parent::__construct($id, $params);
      parent::setModule(new BQueueAreas());

      $this->TableParams = array("ctitle" => null,
				 "titles" => array( _("Id"), _("Name"), _("Map"), _("Status") ),
				 "cols" => array( "id", "name", "map", "status" ),
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
    if($colname == "map")
      {
	return "<img height='40' src='$elt'></img>";
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