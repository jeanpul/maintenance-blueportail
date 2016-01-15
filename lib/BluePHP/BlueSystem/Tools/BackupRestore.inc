<?php

class BackupRestore
{
  private $params = array("MODULE_BACKUP_FILE" => "/etc/BEV/modules/%s/BackupRestore.cfg",
			  "MODULE_CLEAR_FILE" => "/etc/BEV/modules/%s/ClearAction.cfg" );  

  public function haveSomethingToDo($module = "BlueSystem")
  {
    return true;
    return file_exists(sprintf($this->params["MODULE_BACKUP_FILE"], $module)) or
      file_exists(sprintf($this->params["MODULE_CLEAR_FILE"], $module));
  }
}

?>