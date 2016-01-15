<?php

include_once("BluePHP/System/System.inc");
include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/Utils/ModuleProcessing.inc");

/**
 * \class Installation
 * KBEV OS harddrive installation process. Start the 
 * bash installation process and provides the installation
 * status (the installation script writes some information
 * status in the file /tmp/.onInstall).
 * \addtogroup BlueSystem
 * @{
 */
class Installation extends ModuleProcessing
{
  /**
   * Reference to the System
   */
  private $sys = null;

  /**
   * Create the installation
   */
  public function __construct($sharedCon = null)
  {
    parent::__construct(array());
    $this->sys = new System();
  }

  function getEntry($params)
  {
    return array();
  }

  function setEntry($params)
  {
  }

  /**
   * Returns a list of available
   * hd device where we can install the system
   */
  function getHardDisks()
  {
    return $this->sys->getHardDisks();
  }

  /**
   * Check if the system installation
   * is running
   */
  function isRunning()
  {
    return file_exists("/tmp/.onInstall");
  }

  /**
   * Returns an array describing 
   * the system installation status :
   * SYSTEM_INSTALL_STEP = [0..10]
   * SYSTEM_INSTALL_MSG = str
   * onInstall = [true|false]
   * \param $params actually not used
   */
  function checkStatus($params = array())
  {
    $res = array( "SYSTEM_INSTALL_STEP" => 0,
		  "SYSTEM_INSTALL_MSG" => "",
		  "onInstall" => file_exists("/tmp/.onInstall") );
    if($res["onInstall"])
      {
	$output = array();
	exec("cat /tmp/.onInstall", $output);
	$res = array_merge($res, parseSimplePairValues($output));
      }
    return $res;
  }

  /**
   * Start the system installation on the specified
   * hd device
   * \param $params array of parameter with SYSTEM_DEVICE containing the hd device (/dev/sda for example)
   */
  function startInstallation($params)
  {
    if(!file_exists("/tmp/.onInstall"))
      {
	file_put_contents("/tmp/.onInstall", "SYSTEM_INSTALL_STEP=10\nSYSTEM_INSTALL_MSG=Start installation on " . 
			  $params["SYSTEM_DEVICE"]);
	system("sudo /etc/BEV/scripts/BlueSystem/InstallKnoppix.sh /dev/" . $params["SYSTEM_DEVICE"] . " >/dev/null &");
      }
    return array();
  }

};

/** @} */

?>