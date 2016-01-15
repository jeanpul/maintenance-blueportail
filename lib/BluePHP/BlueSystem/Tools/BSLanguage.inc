<?php

include_once('BluePHP/BlueSystem/Tools/Language.inc');
include_once('BluePHP/BlueSystem/Tools/BSConstant.inc');
include_once('BluePHP/BlueSystem/Tools/BSPropertyFile.inc');

function BSLanguage_loadBlueSystemLanguage($textdomain)
{
  $prop = new BSPropertyFile(BLUESYSTEM_LANG_FILE);
  $prop->loadProperties();
  $lang = $prop->getProperty('LANG');
  setLanguage($lang, $textdomain);
}

?>

