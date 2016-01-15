<?php

include_once("BluePHP/Utils/ModuleProcessing.inc");
include_once("Projects.inc");
include_once("Contacts.inc");
include_once("Companies.inc");
include_once("BluePHP/BlueProjectPHP/Config.inc");
include_once("BluePHP/BAM/BAMHttpClient.inc");

class Action extends ModuleProcessing
{
  public function __construct($sharedCon = null)
  {
    parent::setParams(array( "name" => "action",
			     "keyname" => "action_id",
			     "sort" => "date" ),
		      $sharedCon);
  }

  function newEntry($params)
  {
    $ret = parent::newEntry($params);
    // send BAM msg
    $bam = new BAMHttpClient(array("host" => "localhost"));

    $module = new Projects();
    $p = $module->getEntry(array("project_id" => $params["action_project"]));

    $module = new Contacts();
    $c = $module->getEntry(array("contact_id" => $params["contact_id"]));

    $module = new Companies();
    $comp = $module->getEntry(array("company_id" => $p["company_id"]));

    $bam->Msg_info("bev.wiki.actions", "New action from " . $c["contact_first_name"] . " on project " .
    		   $p["project_name"] . " for " . $comp["company_name"]);
    return $params;
  }

  function getSubCategories()
  {
    return parent::Select("* from action_subcat order by name,subcat_id");
  }

  function getActionsFromDate($params)
  {
    return parent::Select("action_id, contact_first_name, project_name, comment from action, contacts, projects where " .
			  " action.contact_id=contacts.contact_id and action_project=project_id and " .
			  " date_format(date, '%Y%m%d')=\"" . $params["date"] . "\" order by action_id desc");
  }

  public function changeCIRValue($params)
  {
    parent::QueryNoReturn("update action set cir='" . $params["cir"] . "' where action_id=" . $params["action_id"]);
  }

}

?>
