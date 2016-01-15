<?php

include_once("BluePHP/BlueProjectPHP/Config.inc");
include_once("BluePHP/Utils/ModuleProcessing.inc");

class Familly extends ModuleProcessing
{
  function Familly()
  {
    parent::setParams(array( "name" => "familly",
			     "keyname" => "familly_id",
			     "sort" => "familly_name" ));
  }

  function getCategories($params)
  {
    return parent::Select("action_cat.cat_id as id, name from action_cat, familly_cat " .
			  "where familly_cat.familly_id=" . $params["action_familly"] . 
			  " and familly_cat.cat_id=action_cat.cat_id order by name");
  }

  function getProjects($params)
  {
    return parent::Select("projects.project_id as id, " .
			  "concat('(',projects.project_id,') ', project_name) as name " .
			  "from projects, familly_projects " . 
			  "where familly_id=" . $params["action_familly"] . 
			  " and familly_projects.project_id=projects.project_id order by projects.project_id desc");
  }
}


?>