<?php

include_once("BluePHP/GUI/ModuleInterface.inc");
include_once("BluePHP/GUI/MenuEntryButton.inc");
include_once("BluePHP/JQUERY/UIButton.inc");
include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/BlueSystem/GUI/BSApplication.inc");
include_once("BluePHP/BlueSystem/GUI/BSModuleInterface.inc");
include_once("BluePHP/BlueSystem/GUI/BSDialogInfo.inc");
include_once("BluePHP/BlueSystem/GUI/BSDialogSubmit.inc");
include_once("BluePHP/BlueSystem/GUI/BSDataTable.inc");


class BSModuleInterface extends ModuleInterface
{
  private $tableParams = array("createLinks" => true,
			       "deleteLinks" => true,	
			       "editLinks" => true,
			       "ctitle" => "",
			       "titles" => null,
			       "cols" => null,
			       "cbId" => null,
			       "cbModule" => null,
			       "refreshButton" => false );
  private $dialogTitles = array( "editTitle" => "", 
				 "createTitle" => "", 
				 "deleteTitle" => "",
				 "tableTitle" => "" );
  private $moduleName = "BlueSystem";
  private $className = "BSModuleInterface";
  private $dialogParams = array();
  private $bsDialogEdit = null;
  private $bsDialogTable = null;
  private $bsTableList = null;

  public function __construct($id, $params)
  {
    parent::__construct($id, $params);
    $this->setContent("BSModuleInterface");
    $this->setBasePath("BluePHP/BlueSystem/GUI/templates");
    $this->setClassName(get_class($this));
    $this->bsDialogEdit = new BSDialogSubmit($this->getClassName() . "Dialog",
					     $this->getDialogParams());
    $this->bsDialogTable = new BSDialogInfo($this->getClassName() . "DialogTable");	    
    $this->bsDialogTable->setWidth("800px");
    $this->bsDataTable = new BSDataTable($this->getClassName() . "Table");
  }

  public function getDialogEdit()
  {
    return $this->bsDialogEdit;
  }

  public function getDialogTable()
  {
    return $this->bsDialogTable;
  }

  protected function getDataTable()
  {
    return $this->bsDataTable;
  }

  public function setEditTitle($t)
  {
    $this->dialogTitles["editTitle"] = $t;
  }

  protected function setCreateTitle($t)
  {
    $this->dialogTitles["createTitle"] = $t;
  }

  protected function setRefreshButton($v)
  {
    $this->tableParams["refreshButton"] = $v;
  }

  protected function setDeleteTitle($t)
  {
    $this->dialogTitles["deleteTitle"] = $t;
  }

  protected function getEditTitle()
  {
    return $this->dialogTitles["editTitle"];
  }

  protected function getCreateTitle()
  {
    return $this->dialogTitles["createTitle"];
  }

  protected function getDeleteTitle()
  {
    return $this->dialogTitles["deleteTitle"];
  }

  protected function getCreateLinksParams()
  {
    return "action=create";
  }

  protected function getCreateLinks($params)
  {
    $me = new UIButton("btn_create",
		       $this->tableParams["ctitle"],
		       array( "onClick" => BS_eventRender( $this->getModuleName() . "." . $this->getClassName(),
							   $this->getCreateLinksParams() )));
    return $me->render();
  }

  protected function getEditLinks($params)
  {
    $keyparam = $params["keyparam"];
    $me = new MenuEntryButton("btn_edit",
			      "<img src='/BTopLocalServer/styles/edit.png'/>",
			      array( "onClick" => BS_eventRender( $this->getModuleName() . "." . $this->getClassName(),
								  "action=edit&${keyparam}" )));
    return $me->render();
  }

  protected function getDeleteLinks($params)
  {
    $keyparam = $params["keyparam"];
    $me = new MenuEntryButton("btn_delete",
			      "<img src='/BTopLocalServer/styles/delete.png'/>",
			      array( "onClick" => BS_eventRenderWithParams( $this->getModuleName() . "." 
									    . $this->getClassName(),
									    "",
									    "action=delete&${keyparam}",
									    "delEntrySubmission" )));
    return $me->render();
  }  

  public function delEntrySubmission()
  {
    $keyparam = $this->getModule()->getURLEntry($this->getParams());
    $this->bsDialogEdit->setTitle($this->getDeleteTitle());
    $this->bsDialogEdit->eventSubmit(BS_eventRenderWithParams($this->getModuleName() . "." . 
					      $this->getClassName(),
					      "", 
					      $keyparam,
					      "delEntry"));
    $this->bsDialogEdit->render();    
  }

  public function setEntry()
  {
    $this->getModule()->setEntry($this->getParams());
    $cb = "\$(document).ready( function() { BlueSystem.ModuleUpdateList({'id':'" . $this->getId() . "'}); });";
    $render = H()->javascript($cb);
    echo $render;
  }

  public function newEntry($params = null)
  {
    $this->getModule()->newEntry($this->getParams());
    $cb = "\$(document).ready( function() { BlueSystem.ModuleUpdateList({'id':'" . $this->getId() . "'}); });";
    $render = H()->javascript($cb);
    echo $render;
  }

  public function delEntry()
  {
    $this->getModule()->delEntry($this->getParams());
    $cb = "\$(document).ready( function() { BlueSystem.ModuleUpdateList({'id':'" . $this->getId() . "'}); });";
    $render = H()->javascript($cb);
    echo $render;
  }

  protected function setCreateLinks($v)
  {
    $this->tableParams["createLinks"] = $v;
  }

  protected function setCreateLinkTitle($t)
  {
    $this->tableParams["ctitle"] = $t;
  }

  protected function setDeleteLinks($v)
  {
    $this->tableParams["deleteLinks"] = $v;
  }

  protected function setEditLinks($v)
  {
    $this->tableParams["editLinks"] = $v;
  }

  protected function setTableParams($p)
  {
    $this->tableParams = array_merge($this->tableParams, $p);
  }

  protected function getTableParams()
  {
    return $this->tableParams;
  }
  
  protected function setModuleName($m)
  {
    $this->moduleName = $m;
  }

  protected function getModuleName()
  {
    return $this->moduleName;
  }

  protected function setClassName($c)
  {
    $this->className = $c;
  }

  protected function getClassName()
  {
    return $this->className;
  }

  protected function setDialogParams($c)
  {
    $this->bsDialogEdit->setDialogParams($c);
  }

  protected function getDialogParams()
  {
    return $this->dialogParams;
  }

  public function getTableList($pagename = "")
  {
    $bs = $this->getDataTable();
    $bs->setContent(parent::getModuleTableList($this->getTableParams(), $pagename));
    $bs->setAjaxSource("/BluePHP/BlueSystem/CallModuleFunction.php?" . 
		       "__module=" . $this->getModuleName() . "&" . 
		       "__class=" . $this->getClassName() . "&" . 
		       "__function=getTableListJSON&" . 
		       "__output=JSON_dataTables");
    $bsTable = $this->getDialogTable();
    $str = H()->div($bs->renderAsStr())
      ->withClass("BSModuleInterface");
    if($this->tableParams["refreshButton"])
      {
	$bsId = $this->getId();
	$func = "BlueSystem.ModuleUpdateList({\"id\":\"$bsId\"});";
	$str->button(_("Refresh"))->withOnClick($func);	
      }
    $bsTable->setMsg($str);
    $bsTable->render();
  }

  public function getTableListJSON($p = null)
  {
    return parent::getModuleTableListJSON($this->getTableParams());
  }

  public function getTableInput($params)   
  {
    $bs = $this->getDialogEdit();

    $cbFunction = "setEntry";
    if($params["action"] == "edit")
      {
	$bs->setTitle($this->getEditTitle());
      }
    else
      {
	$bs->setTitle($this->getCreateTitle());
	$cbFunction = "newEntry";
      }

    $bs->eventSubmit(BS_eventRenderWithForm($this->getModuleName() . "." . 
					    $this->getClassName(),
					    "", 
					    $this->getClassName() . "EditForm",
					    $cbFunction));
    $bs->render();
  }
}

?>