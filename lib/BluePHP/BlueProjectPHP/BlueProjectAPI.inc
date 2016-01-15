<?php

require_once("MySqlOps.inc");
 
function getSelectTODOState($inputName, $defaultState)
{
  $str = '<select name="' . $inputName . '">' . "\n";
  if($defaultState == 0)
    {
      $str .= getOption(0, "A faire", true);
    }
  else
    {
      $str .= getOption(0, "A faire", false);
    }

  if($defaultState == 1)
    {
      $str .= getOption(1, "En cours", true);
    }
  else
    {
      $str .= getOption(1, "En cours", false);
    }
	
  if($defaultState == 2)
    {
      $str .= getOption(2, "Fait", true);
    }
  else
    {
      $str .= getOption(2, "Fait", false);
    }
  
  $str .= "</select>\n";
  return $str;  
}
 
function getFamillyProject($params)
{
  $params["query"] = "SELECT projects.project_id as id, concat('(',projects.project_id,') ', project_name) as name ".
    "FROM projects, familly_projects " .
    "WHERE familly_id=" . $params["selectId"] . " AND familly_projects.project_id=projects.project_id " .
    "ORDER BY project_name";

  return getSelectFromQuery($params);
}

function getProjectCategory($params)
{
  $params["query"] = "SELECT project_cat_id as id, project_cat_id as name FROM projects_cat " .
    "WHERE project_id=" . $params["selectId"] . " ORDER BY project_cat_id";
  
  return getSelectFromQuery($params);
}

function getFamillyCategory($params)
{
  $params["query"] = "SELECT action_cat.cat_id as id, name ".
    "FROM action_cat, familly_cat " .
    "WHERE familly_cat.familly_id=" . $params["selectId"] . " AND familly_cat.cat_id=action_cat.cat_id " .
    "ORDER BY name";

  return getSelectFromQuery($params);
}

function getTasksProject($params)
{
  $params["query"] = "select tasks_id as id, tasks_name as name, project_id " . 
    "FROM tasks_projects where project_id=" . $params["selectId"];
  return getSelectFromQuery($params);
}

function getContactsFromCompany($params)
{
  $params["query"] = "select concat(contact_first_name, ' ', contact_last_name) as name, contact_id as id from contacts where company_id=" . $params["selectId"] . 
    " order by contact_last_name, contact_first_name";
  return getSelectFromQuery($params);
}

function getSelectFromQuery($params)
{
  $str = "";
  if(isset($params["withSelect"]))
    {
      $action = "";
      if(isset($params["action"]))
	{
	  $action = $params["action"];
	}
      $str .= '<select name="' . $params["inputName"] . '" ' . $action . '>' . "\n";
    }
  $result = Query($params["query"], $params["connexion"]);
  while($row = NextObject($result))
    {
      if(isset($params["defaultId"]) and $row->id == $params["defaultId"])
	{
	  $str .= '<option value="' . $row->id . '" selected>' . $row->name . '</option>' . "\n";
	}
      else
	{
	  $str .= '<option value="' . $row->id . '" >' . $row->name . '</option>' . "\n";
	}
    }
  if(isset($params["withSelect"]))
    {
      $str .= "</select>\n";
    }
  return $str;
}

?>