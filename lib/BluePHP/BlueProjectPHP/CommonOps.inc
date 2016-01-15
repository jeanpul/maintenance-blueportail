<?php

/**
 * File: CommonOps
 * Common operations to handle
 * input pages such as inventory, journal...
 * such as insert, update and delete actions
 */

/**
 * Templates definition used everywhere into the BlueProject
 * input files such as journal.php, inventory.php...
 * This contains the relation between a column name and the
 * table and value used to display it.
 */
$templates = array(
		   "project_id" => array( "table" => "projects",
					  "value" => "project_name",
					  "order" => "project_name",
					  "default" => 136
					  ),
		   "contact_id" => array( "table" => "contacts",
					  "value" => "convert(contact_last_name using utf8),convert(contact_first_name using utf8)",
					  "order" => "contact_last_name,contact_first_name",
					  "default" => 26
					  ),
		   "company_id" => array( "table" => "companies",
					  "value" => "company_name",
					  "order" => "company_name"
					  ),
		   "product_id" => array( "table" => "products",
					  "value" => "product_name",
					  "order" => "product_name"
					  ),
		   "sensor_id" => array( "table" => "sensors",
					  "value" => "sensor_ref",
					  "order" => "sensor_ref"
					 ),
		   "cat_id" => array( "table" => "action_cat",
				      "value" => "name",
				      "order" => "name,cat_id"
				      ),
		   "subcat_id" => array( "table" => "action_subcat",
					 "value" => "name",
					 "order" => "name,subcat_id"
					 ),
		   "familly_id" => array( "table" => "familly",
					  "value" => "familly_name",
					  "order" => "familly_name",
					  "default" => 1 ),
		   "project_cat_id" => array( "table" => "projects_cat",
					      "value" => "project_cat_id",
					      "order" => "project_cat_id" ),
		   "vcs_ref" => array( "table" => "VCS",
				       "value" => "ref",
				       "order" => "ref",
				       "where" => "state='new'" ),
		   "vcs_product" => array( "table" => "VCS_Products",
					   "value" => "ref",
					   "order" => "ref" ),
		   "vcs_vtype" => array( "table" => "vtype",
					 "value" => "name",
					 "order" => "name" )
		   );


function handleAction2($params, $module, $page)
{
  $str = "";
  $params = array_merge( array( "action" => "new" ),
			 $params);

  if($params["action"] == "new")
    {
      $str = getForm(getInput(array( "type" => "hidden", "name" => "action", "value" => "insert" )) .
      		     $module->newEntry($params) .
		     getInput(array( "type" => "submit", "value" => "Apply" )),
		     array( "method" => "get", "action" => $page, "name" => "action" ));

    }
  else if($params["action"] == "edit")
    {
      $str = getForm(getInput(array( "type" => "hidden", "name" => "action", "value" => "update" )) .
		     $module->editEntry($params) .
		     getInput(array( "type" => "submit", "value" => "Apply" )),
		     array( "method" => "get", "action" => $page ));     
    }
  else if($params["action"] == "insert")
    {
      $str = $module->insertEntry($params);
    }
  else if($params["action"] == "update")
    {
      $str = $module->updateEntry($params);
    }
 else if($params["action"] == "delete")
   {
     $str = $module->deleteEntry($params);
   }
  
  return $str;
}


function handleAction($table, $params, $keyname, $connexion)
{
  $str = "";

  $action = "new";

  if(isset($params) and isset($params["action"]))
    {
      $action = $params["action"];
    }
  else
    {
      // bizarre mais on s'assure
      // que ce parametre existe car il est utilise
      // dans d'autres fonctions
      $params["action"] = $action; 
    }

  if($action == "new")
    {
      $str .= "<h1>Insert a new entry into table $table</h1>\n";
      $str .= handleInput($table, $params, $keyname, $connexion);
      return $str;
    }
  else if($action == "edit")
    {
      $str .= "<h1>Edit an existing entry into table $table</h1>\n";
      $str .= handleInput($table, $params, $keyname, $connexion);
      return $str;
    }
  else if($action == "insert")
    {
      if(insertAssoc($table, $params, $connexion))
	{
	  return "<h1>Entry inserted.</h1>\n";
	}
      else
	{
	  return "<h1>Error during insertion.</h1>\n";
	}
    }
  else if($action == "update")
    {
      if(updateAssoc($table, $params, $keyname, $connexion))
	{
	  return "<h1>Entry updated.</h1>\n";
	}
      else
	{
	  return "<h1>Error during update.</h1>\n";
	}
    }
  else if($action == "delete")
    {
      if(deleteWithKey($table, $keyname, $params[$keyname], $connexion))
	{
	  return "<h1>Entry deleted.</h1>\n";
	}
      else
	{
	  return "<h1>Error during deletion.</h1>\n";
	}      
    }
  return $str; 
}

function handleInput($table, $params, $keyname, $connexion)
{
  $str = "";

  $assoc = array();

  if($params["action"] == "edit")
    {
      $query = "SELECT * FROM $table WHERE $keyname=" . $params[$keyname];
      $result = Query($query, $connexion);
      $assoc = NextAssoc($result); 
    }
  else
    {
      $query = "SHOW COLUMNS FROM $table";
      $result = Query($query, $connexion);
      while($columns =  NextAssoc($result))
	{
	  $assoc[$columns["Field"]] = isset($params[$columns["Field"]]) ? $params[$columns["Field"]] : "";
	}
    }

  if($params["action"] == "edit")
    {
      $str .= '<form method="get" action="' . $table . '.php" name="' . $table . '">' . "\n";
      $str .= '<input type="hidden" name="' . $keyname . '" value="' . $assoc[$keyname] . '"/>' . "\n";
      $str .= '<input type="hidden" name="action" value="update"/>' . "\n";
    }
  else
    {
      $str .= '<form method="get" action="' . $table . '.php" name="' . $table . '">' . "\n";
      $str .= '<input type="hidden" name="action" value="insert"/>' . "\n";
    }

  $str .= getTableInput($assoc, $connexion);

  $str .= "<input type=SUBMIT>\n"; 
  $str .= "</form>\n";

  return $str;
}



function createJSCAll($f, $params, $action="onChange")
{
  $fcall = $action . "='$f(";
  $firstComa = true;
  foreach($params as $k => $v)
    {
      if($firstComa)
	{
	  $firstComa = false;
	}
      else
	{
	  $fcall .= ",";
	}
      $fcall .= "\"$v\"";
    }
  $fcall .= ")'";
  return $fcall;
}

function sendEmail($to, $subject, $content)
{
  $mailHeaders = "From: blueproject@blueeyevideo.com" . "\r\n" .
    'Reply-To: support@blueeyevideo.com' . "\r\n" .
    'X-Mailer: PHP/' . phpversion();
  return mail($to, $subject, $content, $headers);
}

?>