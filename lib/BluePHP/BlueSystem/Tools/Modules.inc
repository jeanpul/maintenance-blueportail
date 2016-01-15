<?php

/**
 * \class Modules
 * \deprecated Not used anymore
 */
class Modules
{
  /**
   * Module file path
   */
  private $params = array( "LIST_FILE" => "/etc/BEV/modulesDisplay.list" );

  /**
   * Returns if the system has available modules
   */
  public function haveModules()
  {
    return (file_exists($this->params["LIST_FILE"]) and 
	    filesize($this->params["LIST_FILE"]) > 0);
  }
}

?>
