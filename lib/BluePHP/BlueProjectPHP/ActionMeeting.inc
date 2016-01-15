<?php

include_once("BluePHP/ModuleProcessing.inc");

class ActionMeeting extends ModuleProcessing
{
  private $date = null;
  
  public function __construct($date = null)
  {
    parent::SetParams(array( "name" => "project_history",
			     "keyname" => array( "project_id", "date" ),
			     "sort" => array( "urgent", "priority" )));
    if(!is_null($date))
      {
	$this->date = $date;
      }
    else
      {
	$this->date = time();
      }
  }

  public function getDate()
  {
    return strftime("%m-%d-%Y", $this->date);
  }

  public function getWeek()
  {
    return strftime("%V", $this->date);
  }

  public function getEntries()
  {
    return parent::Select("urgent, priority, company_final_id, project_name, project_company, " . 
			  "project_history.project_id as project_id, " . 
			  "status, date from project_history, projects " . 
			  "where project_history.project_id=projects.project_id " . 
			  "order by urgent, priority");
  }    
}

?>
