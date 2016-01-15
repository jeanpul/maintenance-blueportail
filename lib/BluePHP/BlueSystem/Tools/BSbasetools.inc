<?php
include_once("BluePHP/BlueSystem/Tools/BSConstant.inc");
include_once("BluePHP/BlueSystem/Tools/BSPropertyFile.inc");

function BSbasetools_obtainHostClientId()
{
  $pfile = new BSPropertyFile(BLUESYSTEM_PROPERTIES_FILE);
  $pfile->loadProperties();
  return $pfile->getProperty('BLUESYSTEM_CLIENTID');
}

function BSbasetools_obtainMacAdress($interface = "eth0")
{
  $mac = "";
  exec('/sbin/ifconfig ' . $interface, $output);
  $output = preg_grep('/HWaddr/', $output);
  if(count($output))
    {
      $mac = preg_replace('/.*HWaddr .*(..:..:..:..:..:..).*/', '$1', $output[0]);
      $mac = strtoupper($mac);
    }
  return $mac;
}

function BSbasetools_getReference()
{
  $mac = BSbasetools_obtainMacAdress("eth0");
  if($mac == "") $mac = "none";

  $pfile = new BSPropertyFile(BLUESYSTEM_PROPERTIES_FILE);
  $pfile->loadProperties();

  $ref = 'Ref: ' . $pfile->getProperty('BLUESYSTEM_APPLI') .' / ';
  $ref .= $pfile->getProperty('BLUESYSTEM_VERSION') .' Rev. ';
  $ref .= $pfile->getProperty('BLUESYSTEM_REVISION').' / ';
  $ref .= $mac;
  return $ref;
}

function BSbasetools_getCopyright()
{
  $msg = _("%d BlueEyeVideo All rights reserved");
  return sprintf($msg,  BLUESYSTEM_COPYRIGHT_YEAR);
}

function BSbasetools_recursiveRemove($d)
{
  if($is_dir($d))
    {
      $files = scandir($d);
      foreach($files as $f)
	{
	  BSbasetools_recursiveRemove($f);
	}
    }
  unlink($d);
}

?>
