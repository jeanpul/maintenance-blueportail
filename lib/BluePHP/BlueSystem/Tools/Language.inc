<?php

function setLanguage($lang = "", $textdomain = "")
{
  if(!$lang)
    {
      if(!defined('LANG')) die("LANG not defined");
      $lang = LANG;
    }

  if(!$textdomain and defined('TEXT_DOMAIN'))
    {
      $textdomain = TEXT_DOMAIN;
    }

  setlocale(LC_ALL, $lang . ".utf8");
  textdomain($textdomain);
  if($lang == "fr_FR" || $lang == "en_US" || $lang == "es_ES")
  {
    define(LANG_CHARSET, "UTF-8");
  }
  else
  {
    define(LANG_CHARSET, "ISO8859-1");
  }
}


?>