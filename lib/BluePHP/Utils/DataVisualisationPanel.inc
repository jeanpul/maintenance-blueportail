<?php

require_once("BluePHP/GUI/Frame.inc");
require_once("BluePHP/Utils/Forms.inc");
require_once("BluePHP/Utils/Input.inc");
require_once("BluePHP/Utils/DataAction.inc");

/**
 * \class DataVisualisationPanel
 * Base class "interface" for every class that manage some data
 * and provides a GUI
 * \addtogroup Utils
 * @{
 */
class DataVisualisationPanel extends Frame implements DataAction
{
  /**
   * Module parameters
   */
  private  $params = array( "__module" => false,
			    "name" => false,
			    "pagename" => false,
			    "titles" => array( "list" => false, 
					       "edit" => false,
					       "new" => false ));
  /**
   * \deprecated should not be used anymore ?
   */
  var $Fields = array( "labels" => array(),
		       "render" => array() );

  public function __construct($id, $params)
  {
    parent::__construct($id, "DataVisualisationPanel", "BluePHP/GUI/templates");
    $this->params = array_merge( array( "action" => "list" ), $params );
  }

  public function isRenderList()
  {
    return $this->params["action"] == "list";
  }

  public function isRenderEdit()
  {
    return $this->params["action"] == "edit";
  }

  public function renderList()
  {
    echo $this->getTableList();
  }

  public function renderEdit($params)
  {
    echo $this->getTableInput($params);
  }

  /**
   * Set the module name
   * \param $name module name
   */
  function setName($name)
  {
    $this->params["name"] = $name;
  }

  /**
   * Returns the module name
   */
  function getName()
  {
    return $this->params["name"];
  }

  /**
   * Returns the module params
   */
  protected function getParams()
  {
    return $this->params;
  }

  /**
   * Set the module processing interface
   * \param $module processing interface
   */
  protected function setModule($module)
  {
    $this->params["__module"] = $module;
  }

  /**
   * Get the module processing interface
   */
  protected function getModule()
  {
    return $this->params["__module"];
  }

  /**
   * Returns hidden input values
   * for keys
   * \param $params key elements
   */
  function getFormInput($params)
  {
    $str = "";
    $keys = explode(",", $this->getModule()->getKey());
    foreach($keys as $k => $v)
      {
	$str .= $this->getHiddenInput($v, $params["$v"]);
      }
    return $str;
  }

  /**
   * \warning what's this for ?
   */
  function addRender($field, $cb)
  {
    $this->Fields["render"][$field] = $cb;
  }

  /**
   * \warning what is it ?
   */
  function addLabel($field, $label)
  {
    $this->Fields["labels"][$field] = $label;
  }

  /**
   * Render the GUI to edit a particular data entry
   * \param $params data values
   */
  function editEntry($params)
  {
    $params = array_merge($params, $this->getModule()->getEntry($params));
    return $this->getEditTitle($this->getName()) . $this->getTableInput($params);
  }

  /**
   * Render the GUI to add a new entry
   * \param $params data values
   */
  function newEntry($params)
  {
    $params = array_merge($params, $this->getModule()->getEntry($params));
    return $this->getNewTitle($this->getName()) . $this->getTableInput($params);    
  }

  /**
   * Call the module processing to add a new entry
   * then return the result to the GUI
   * \param $params data values
   */
  function insertEntry($params)
  {
    return $this->getInsertMsg($this->getModule()->newEntry($params));
  }

  /**
   * Call the module processing to update a new entry
   * then return the result to the GUI
   * \param $params data values
   */
  function updateEntry($params)
  {
    return $this->getUpdateMsg($this->getModule()->setEntry($params));
  }

  /**
   * Call the module processing to remove an entry
   * then return the result to the GUI
   * \param $params data values
   */
  function deleteEntry($params)
  {
    return $this->getDeleteMsg($this->getModule()->delEntry($params));
  }

  /**
   * \deprecated do not use this anymore
   */
  function getEditTitle($name)
  {
    return "<h2>Edit an existing $name</h2>\n";
  }

  /**
   * \deprecated do not use this anymore
   */
  function getNewTitle($name)
  {
    return "<h2>Add a new $name</h2>\n";
  }

  /**
   * \deprecated do not use this anymore
   */
  function getInsertMsg($res)
  {
    return $res ? "<h2>Entry inserted.</h2>\n" : "<h2>Error during insertion.</h2>\n";
  }
  
  /**
   * \deprecated do not use this anymore
   */
  function getUpdateMsg($res)
  {
    return $res ? "<h2>Entry updated.</h2>\n" : "<h2>Error during update.</h2>\n";
  }

  /**
   * \deprecated do not use this anymore
   */
  function getDeleteMsg($res)
  {
    return $res ? "<h2>Entry deleted.</h2>\n" : "<h2>Error during deletion.</h2>\n";
  }
  
  /**
   * Returns an input hidden
   * \param $name input name
   * \param $value input value
   */
  function getHiddenInput($name, $value)
  {
    return createInput(array( "type" => "hidden",
			      "name" => $name,
			      "value" => $value ));
  }

  /**
   * Render the appropriate GUI regarding
   * the page parameters :
   * where action = "new" means create a new element
   *                "edit" edit an existing element
   *                "insert" insert the element from the filled form
   *                "update" update the element from the filled form
   *                "delete" delete an existing element
   * \param $params data values
   * \param $page page reference
   * \param $extraInputs add some form elements to the default ones
   */
  function handleAction($params, $page, $extraInputs = "")
  {
    $str = "";
    $params = array_merge( array( "action" => "new" ),
			   $params);   

    if($params["action"] == "new")
      {
	$str = createForm(createInput(array( "type" => "hidden", "name" => "action", "value" => "insert" )) .
			  $extraInputs . 
			  $this->newEntry($params) .
			  createInput(array( "type" => "submit", "value" => "Apply" )),
			  array( "method" => "get", "action" => $page, "name" => "action" ));
	
      }
    else if($params["action"] == "edit")
      {
	$str = createForm(createInput(array( "type" => "hidden", "name" => "action", "value" => "update" )) .
			  $extraInputs .
			  $this->getFormInput($params) . 
			  $this->editEntry($params) .
			  createInput(array( "type" => "submit", "value" => "Apply" )),
			  array( "method" => "get", "action" => $page ));     
      }
    else if($params["action"] == "insert")
      {
	$str = $this->insertEntry($params);
      }
    else if($params["action"] == "update")
      {
	$str = $this->updateEntry($params);
      }
    else if($params["action"] == "delete")
      {
	$str = $this->deleteEntry($params);
      }

    return $str;
  }

  /**
   * Returns the html button links that raise the GUI to create
   * a new entry
   * \param $params module parameters
   */
  protected function getCreateLinks($params)
  {
    return "<button class='btn' onclick=\"BlueSystem.ModuleWindowStart({id:'" . $params["cbId"] . "',module:'" . $params["cbModule"] . "'}" . 
      ",'?action=new');\">" . $params["ctitle"] . "</button>";
  }

  /**
   * Returns the html button links that raise the GUI to edit
   * an existing entry
   * \param $params module parameters
   */
  protected function getEditLinks($params)
  {
    $keyparam = $params["keyparam"];
    return "<button onclick=\"BlueSystem.ModuleWindowStart({id:'" . $params["cbId"] . "',module:'" . $params["cbModule"] . "'}" . 
      ",'?action=edit&${keyparam}');\"><img src='/BTopLocalServer/styles/edit.png'/></button>";
  }

  /**
   * Returns the html button links used to delete an existing
   * entry
   * \param $params module parameters
   */
  protected function getDeleteLinks($params)
  {
    $keyparam = $params["keyparam"];
    return "<button onclick=\"BlueSystem.ModuleWindowDelete({id:'" . $params["cbId"] . "',module:'" . $params["cbModule"] . "'}" . 
      ",'?action=delete&${keyparam}');\"><img src='/BTopLocalServer/styles/delete.png'/></button>";
  }

  /**
   * Create the appropriate links if necessary for
   * the table list entry GUI
   * \param $elt current row
   * \param $params module params
   */
  protected function renderActions($elt, $params)
  {
    $str = "";
    if($params["editLinks"])
      {
	$str .= $this->getEditLinks($params);
      }
    if($params["deleteLinks"])
      {
	$str .= $this->getDeleteLinks($params);
      }
    return $str;
  }
  
  /**
   * Whether or not the module provides actions on entries 
   * \param $params module params
   */
  protected function haveActions($params)
  {
    return $params["editLinks"] or $params["deleteLinks"];
  }

  /**
   * This method could be overloaded to
   * change the element content as it
   * will appear in the table. By default,
   * just return the content.
   * \param $colname current colname
   * \param $elt current cell value
   */
  protected function renderCol($colname, $elt)
  {
    return $elt;
  }

  /**
   * Render the GUI that shows data entries in a table
   * \param $params module params
   * \param $pagename current page
   */
  protected function getModuleTableList($params, $pagename)
  {
    $params = array_merge(array( "createLinks" => true,
				 "editLinks" => true,
				 "deleteLinks" => true ), $params );
    $pageData = array_merge( array( "query" => "" ), parse_url($pagename) );
    $str = $params["createLinks"] ? ($this->getCreateLinks($params) . "\n") : "";
    $str .= "<table id=\"" . $this->getId() . "Table\" class=\"display\">\n";
    $str .= "<thead>\n";
    $str .= "<tr>";
    foreach($params["titles"] as $k => $v)
      {
	$str .= "<th>$v</th>";
      }
    // add edit and delete actions titles
    if($this->haveActions($params))
      {
	$str .= "<th>" . _("Actions") . "</th>";
      }
    $str .= "</tr></thead>\n";
    $str .= "<tbody>\n";
    $data = $this->getModule()->getEntries();
    foreach($data as $k => $v)
      {
	$str .= "<tr>";
	foreach($params["cols"] as $l => $w)
	  {
	    $str .= "<td>" . $this->renderCol($l, $v[$w]) . "</td>";
	   }
	// add edit and delete actions links
	// FAB : this part must be defined into a separate method
	// that could be overloaded as renderActions
	$params["keyparam"] = $this->getModule()->getKeyCond($v, "&", false);
	if($this->haveActions($params))
	  {
	    $str .= "<td>" . $this->renderActions($v, $params) . "</td>";
	  }
	$str .= "</tr>\n";
      }
    $str .= "</tbody>\n";
    $str .= "</table>\n";
    return $str;
  }

  /**
   * Return the entries values in a JSON format so that
   * you can create or update a table using AJAX
   * \todo FAB: FACTORISATION MUST BE DONE between this method and the getModuleTableList HTML
   * \param $params module parameter
   */
  protected function getModuleTableListJSON($params)
  {
    $params = array_merge(array( "createLinks" => true,
				 "editLinks" => true,
				 "deleteLinks" => true ), $params );
    $data = $this->getModule()->getEntries();
    $res = array();
    foreach($data as $elt)
      {
	$row = array();
	for($i = 0; $i < count($params["cols"]); ++$i)
	  {
	    $row[] = $this->renderCol($params["cols"][$i], $elt[$params["cols"][$i]]);	    
	  }
	$params["keyparam"] = $this->getModule()->getKeyCond($elt, "&", false);
	if($this->haveActions($params))
	  {
	    $row[] = $this->renderActions($elt, $params);
	  }
	$res[] = $row;
      }
    return $res;
  }

  /**
   * \deprecated not used anymore ?
   */
  function getLabel($field)
  {
    return isset($this->Fields["labels"][$field]) ? $this->Fields["labels"][$field] : $field;
  }

  /**
   * \deprecated not used anymore ?
   */
  function getRender($field)
  {
    return isset($this->Fields["render"][$field]) ? $this->Fields["render"][$field] : null;
  }


  /**
   * Return the GUI to edit or create a new data entry
   * \param $params module parameter
   */
  protected function getTableInput($params)
  {
    $st = $this->getModule()->getModuleStructure();
    $str = "<table>\n";
    foreach($st as $k => $v)
      {
	$v = array_merge(array( "Extra" => null ), $v);
	if($params["action"] != "new" or ($params["action"] == "new" and $v["Extra"] != "auto_increment"))
	  {
	    $readonly = ($v["Extra"] == "auto_increment");
	    if($v["Type"] == "enum")
	      {
		$str .= createTableRow(array( $this->getLabel($v["Field"]), 
					      createSelect($v["Elts"],
							   array( "value" => false,
								  "name" => $k,
								  "key" => false,
								  "default" => $params[$k],
								  "extra" => false ))));
	      }
	    else if($v["Type"] == "datetime")
	      {
		$str .= createTableRow(array( $this->getLabel($v["Field"]), 
					      createInputDate($params[$k],
							      array( "name" => $k,
								     "title" => "cal",
								     "format" => "yyyy-MM-dd"))));
	      }
	    else if($v["Type"] == "text")
	      {
		$str .= createTableRow(array( $this->getLabel($v["Field"]), 
					      createInputTextArea($params[$k], 
								  array( "name" => $k,
									 "rows" => 4, 
									 "cols" => 40, 
									 "extra" => false ))));		
	      }
	    else
	      {
		$size = null;
		if(isset($v["Size"]))
		  {
		    $size = $v["Size"];
		  }
		$str .= createTableRow(array( $this->getLabel($v["Field"]), 
					      createInputText($params[$k],
							      array( "name" => $k, 
								     "readonly" => $readonly,
								     "maxlength" => $size,
								     "extra" => false))));
	      }
	  }
      }
    return $str . "</table>\n";
  }
}

/** @} */

?>
