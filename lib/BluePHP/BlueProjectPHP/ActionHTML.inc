<?php

include_once("BluePHP/Utils/HTML.inc");
include_once("BluePHP/BlueSystem/GUI/BSModuleInterface.inc");
include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/BlueProjectPHP/Action.inc");
include_once("BluePHP/BlueProjectPHP/Contacts.inc");
include_once("BluePHP/BlueProjectPHP/Projects.inc");
include_once("BluePHP/BlueProjectPHP/Familly.inc");

class ActionHTML extends BSModuleInterface
{
  public function __construct($id = "", $params = array())
  {    
    parent::__construct($id, $params);
    parent::setModuleName("BlueProjectPHP");
    parent::setModule(new Action());
    $this->setCreateLinkTitle(_("Create a new Action"));
  }

  public function getTableInput($params)
  {
    $params = array_merge( array( "action_id" => null,
				  "contact_id" => null,
				  "action_familly" => null,
				  "cat_id" => null,
				  "action_project" => null,
				  "subcat_id" => null,
				  "cat_name" => null ),
			   $params );
				  
    $rows = H();

    $readOnly = false;
    if(isset($params["action_id"]))
      {
	$readOnly = true;
      }

    $str = "<table>";

    if($readOnly)
      {
	$rows->row( array( _("Id"),
			  createInputText($params["action_id"],
					  array( "name" => "action_id",
						 "readonly" => true,
						 "maxlength" => 8 ))));
      }

    $module = new Contacts();
    $rows->row( array("Employee", 
		     createSelect($module->getEmployees(),					      
				  array( "value" => "contact_first_name,contact_last_name",
					 "name" => $module->getKey(),
					 "key" => $module->getKey(),
					 "default" => $params[$module->getKey()],
					 "extra" => false ))));    

    $module = new Familly();
    $fam = $module->getEntries();
    if($params["action_familly"] == null)
      {
	$params["action_familly"] = $fam[0]["familly_id"];
      }
    $rows->row( array("Familly", 
		     createSelect($module->getEntries(),
				  array( "id" => "action_familly",
					 "value" => "familly_name",
					 "name" => "action_familly",
					 "key" => $module->getKey(),
					 "default" => $params["action_familly"],
					 "extra" => createJSCall("changeProjectAndCategory", array())))));
    
    $rows->row( array("Familly Category", 
		     createSelect($module->getCategories($params),
				  array( "value" => "name", 
					 "name" => "cat_id", 
					 "key" => "id", 
					 "default" => $params["cat_id"],
					 "extra" => false ))));

    $proj = $module->getProjects($params);
    if($params["action_project"] == null)
      {
	$params["action_project"] = $proj[0]["id"];
      }
    $rows->row( array("Familly Project", 
		     createSelect($module->getProjects($params),
				  array( "value" => "name",
					 "name" => "action_project",
					 "key" => "id",
					 "default" => $params["action_project"],
					 "extra" => createJSCall("changeSubCategory", array())))));
    
    $module = new Projects();
    $rows->row( array("Project Category", 
		     createSelect($module->getCategories($params),
				  array( "value" => "name",
					 "name" => "cat_name",
					 "key" => "id",
					 "default" => $params["cat_name"],
					 "extra" => false ))));

    $rows->row( array("Date", createInputDatePicker($params["date"], 
						    array("name" => "date",
							  "id" => "date",
							  "format" => "yy-mm-dd 00:00:00" ))));

    $module = new Action();
    $rows->row( array("Sub-Category", createSelect($module->getSubCategories(),
						  array( "value" => "name",
							 "name" => "subcat_id",
							 "key" => "subcat_id",
							 "default" => $params["subcat_id"],
							 "extra" => false ))));    

    $rows->row( array("Description", 
		     createInputTextArea($params["action"] == "new" ? "" : $params["comment"],
					 array( "name" => "comment", 
						"rows" => 4, 
						"cols" => 40, 
						"extra" => false ))));
    
    $table = H()->table($rows);
    $gui = H()->form($table)->withId($this->getClassName() . "EditForm");
    
    $this->setDialogParams( array( "msg" => $gui->str() ));
    $this->setEditTitle(_("Edit Action parameters"));
    $this->setCreateTitle(_("Create a new Action"));

    $bsDialog = $this->getDialogEdit();
    $editForm = $this->getClassName() . "EditForm";
    $bsDialog->addSubmitButton("Save as new", "%savenew%");
    $bsDialog->eventSubmitWithName("%savenew%", BS_eventRenderWithForm($this->getModuleName() . "." . 
								       $this->getClassName(),
								       "",
								       $this->getClassName() . "EditForm",
								       "saveAsNew"));
    parent::getTableInput($params);
  }

  public function newEntry($params = null)
  {
    $this->getModule()->newEntry($this->getParams());
    $cb = "\$(document).ready( function() { updateActivityTable(); });";
    $render = H()->javascript($cb);
    echo $render;
  }

  public function setEntry($params = null)
  {
    $this->getModule()->setEntry($this->getParams());
    $cb = "\$(document).ready( function() { updateActivityTable(); });";
    $render = H()->javascript($cb);
    echo $render;
  }

  public function saveAsNew($params = null)
  {
    $params = $this->getParams();
    $this->getModule()->newEntry(array( "contact_id" => $params["contact_id"],
					"action_familly" => $params["action_familly"],
					"cat_id" => $params["cat_id"],
					"action_project" => $params["action_project"],
					"date" => $params["date"],
					"subcat_id" => $params["subcat_id"],
					"comment" => $params["comment"]));
    $cb = "\$(document).ready( function() { updateActivityTable(); });";
    $render = H()->javascript($cb);
    echo $render;
  }
}

?>
