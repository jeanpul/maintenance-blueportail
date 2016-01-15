<?php

/**
 * \class License
 * \todo check if it's used or not
 */
class License
{
  /**
   * license tool path
   */
  private $params = array( "ACTIONS_FILE" => "/etc/BEV/modules/License/StatusActions.cfg" );

  /**
   * Returns if the system have license tools
   */
  public function haveLicenseTools()
  {
    return file_exists($this->params["ACTIONS_FILE"]);
  }
}

?>
