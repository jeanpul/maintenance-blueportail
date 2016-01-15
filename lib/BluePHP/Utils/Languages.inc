<?php

/* 
 * Languages.inc
 *
 * Author: Fabien Pelisson
 * Copyright: 2003 Blue Eye Video
 * 
 * All the translations indexed by an unique term 
 * that is referred into the code.
 * function to configure the language to use and where find the traduction
 * 
 * Sebastien :
 * Utilisation de gettext, textFromId plus utilise
 * Ajout d'une fonction pour positionner le langage en fonction de la config (constante LANG, TEXT_DOMAIN)
 * ajout fonction myhtmlentites a utiliser a la place de htmlentities car positionne le domaine de caractere a remplacer
 * en fonction de la langue
 */

/**
 * LANG
 * TEXT_DOMAIN
 * 
 *
 * LANG_CHARSET defined during the call to setLanguage
 */

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

  setlocale(LC_ALL, $lang . "." . LANG_CHARSET);
  textdomain($textdomain);
}

function myhtmlentities($str)
{
  return htmlentities($str, ENT_COMPAT, LANG_CHARSET);
}

/*
include_once("Config.inc");

$messages = array (
    'en_US'=> array(
       'ConnectionTitle' => '',
**       'Menu' => 'Returns to main menu',
--       'Help' => 'Help',
       'MenuTitle' => 'Select a fonctionality',
--       'Attractivity' => "Attractivity",
--       'Frequency' => "Frequency",
--       'Stops' => "Stops",
--       'MeanFlow' => "Mean Flow",
       'Comparisons' => "Comparisons",
--       'StopPoints' => "Stop Points",
-- FrequencyStop
ConfPAClass
ConfPeriods
Periods
       'CountTitle' => 'Counting values results used from %s to %s between %s to %s',
       'CountJSM' => 'Day/Week/Month counting',
       'CountJJJ' => 'Days counting',
       'CountSSS' => 'Weeks counting',
       'CountMMM' => 'Months counting',
       'FATTitle' => 'Waiting queues results',
       'CountFAT' => 'Waiting queues',
       'OccTitle' => 'Shop occupancy results',
       'CountOcc' => 'Shop occupancy',
       'CameraViewer' => 'Camera View',
--       'Print' => 'Print',
--       'Export' => 'Export',
--       'Quit' => 'Quit',
       'Next' => 'Next',
       'Previous' => 'Previous',
**       'Login' => 'Login',
**       'Passwd' => 'Mot de passe',
**       'Connect' => 'Connect',
**       'Enter' => 'Enter your login and your password',
       'Today' => 'Today',
       'ThisWeek' => 'This week',
       'ThisMonth' => 'This month',
       'Customers' => 'Customers',
       'Entries' => 'Entries',
       'Exits' => 'Exits',
       'Occupancy' => 'Occupancy',
       'Buyers' => 'Buyers',
       'Free' => 'Free',
       'Transfert' => 'Transfert %',
       'Monday' => 'Monday',
       'Tuesday' => 'Tuesday',
       'Wednesday' => 'Wednesday',
       'Thursday' => 'Thursday',
       'Friday' => 'Friday',
       'Saturday' => 'Saturday',
       'Sunday' => 'Sunday',
       'Yesterday' => 'Last %s',
       'TwoDaysAgo' => 'Last %s',
       'LastWeek' => 'Last week',
       'TwoWeeksAgo' => 'Two weeks ago',
       'LastMonth' => 'Last month',
       'TwoMonthsAgo' => 'Two months ago',
       'Duration' => 'Duration (min)',
       'Hours' => 'Hours',
       'Alarm' => 'Send an email for duration > 30',
**       'ErrorMsg' => "The were problems with the previous action :",
**       'ErrorAccess %s' => "Multiple access invalid. A session is still in process click <a href='%s'>here</a> to continue.",
**       'ErrorSession' => "Expired or unauthorized session",
**       'ErrorLogin' => "Incorrect login or password",
       'CountJHistory' => "Counting History",
       'Configuration' => "Counting Configuration",
       'for' => " for ",
**       'Apply' => "Apply",
**       'Functionalities' => "Functionalities",
--       'Tools' => "Tools",
--       'DateSelection' => "Date selection",
**       'ForTheDay' => "For the day",
**       'ForThePeriod' => "For the period",
**       'PeriodFrom' => "from",
**       'PeriodTo' => "to",
--       'NumberEntry' => "Number of entries",
--       'Next day' => "Next day",
--       'Previous day' => "Previous day"
    ),

    /ConfP'fr_FR'=> array(
       'ConnectionTitle' => 'Fenêtre de connexion',
       'Menu' => 'Revenir au menu principal',
       'Help' => 'Aide',
       'MenuTitle' => "Choisissez une fonctionalité",
       'Attractivity' => "TAUX D'ATTRACTIVITE",
       'Frequency' => "TAUX DE FREQUENTATION",
       'FrequencyStop' => "TAUX DE FREQUENTATION ET D'ARRETS",
       'Stops' => "TAUX D'ARRETS + de 5 sec",
       'MeanFlow' => "FLUX MARCHAND MOYEN",
       'Comparisons' => "Comparaisons",
       'StopPoints' => "POINTS D'ARRETS",
       'CountTitle' => 'Résultats des compteurs actifs du %s au %s entre %s et %s',
       'CountJSM' => 'Comptage Jour/Semaine/Mois',
       'CountJJJ' => 'Comptage sur plusieurs jours',
       'CountSSS' => 'Comptage sur plusieurs semaines',
       'CountMMM' => 'Comptage sur plusieurs mois',
       'FATTitle' => 'Temps d\'attentes maximum et nombre de files d\'attente sur la journée',
       'CountFAT' => 'Files d\'attente',
       'OccTitle' => 'Résultats de l\'occupation cumulée du magasin sur la journée',
       'CountOcc' => 'Occupation du magasin',
       'CameraViewer' => 'Vue Camera',
       'Print' => 'Imprimer',
       'Export' => 'Exporter',
       'Quit' => 'Quitter',
       'Next' => 'Suivant',
       'Previous' => 'Précédent',
       'Login' => 'Login',
       'Passwd' => 'Mot de passe',
       'Connect' => 'Connexion',
       'Enter' => 'Saissisez votre login et mot de passe',
       'Today' => 'Aujourd\'hui',
       'ThisWeek' => 'Cette semaine',
       'ThisMonth' => 'Ce mois-ci',
       'Customers' => 'Visiteurs',
       'Entries' => 'Entrées',
       'Exits' => 'Sorties',
       'Occupancy' => 'Occupation',
       'Buyers' => 'Acheteurs',
       'Free' => 'Sans achat',
       'Transfert' => '% de transformation',
       'Monday' => 'Lundi',
       'Tuesday' => 'Mardi',
       'Wednesday' => 'Mercredi',
       'Thursday' => 'Jeudi',
       'Friday' => 'Vendredi',
       'Saturday' => 'Samedi',
       'Sunday' => 'Dimanche',
       'Yesterday' => '%s dernier',
       'TwoDaysAgo' => '%s dernier',
       'LastWeek' => 'La semaine dernière',
       'TwoWeeksAgo' => 'Il y a deux semaines',
       'LastMonth' => 'Le mois dernier',
       'TwoMonthsAgo' => 'Il y a deux mois',
       'Duration' => 'Durées (min)',
       'Hours' => 'Horaires',
       'Alarm' => 'Envoie d\'un e-mail sur une durée > 30',
       'ErrorMsg' => "L'erreur suivante est survenue :",
       'ErrorAccess' => "Acc&eacute;s concurrent interdit. Cliquer <a href='%s'>ici</a> pour continuer.",
       'ErrorSession' => "Session expir&eacute;e ou non autoris&eacute;e",
       'ErrorLogin' => "Login ou Mot de passe incorrect",
       'CountJHistory' => "Historique de comptage",
       'Configuration' => "Configuration du comptage",
       'for' => " pour le ",
       'Apply' => "Appliquer",
       'Functionalities' => "Fonctionnalités",
       'Tools' => "Outils",
       'DateSelection' => "Sélection par date",
       'ForTheDay' => "Pour la journée du",
       'ForThePeriod' => "Pour la période",
       'PeriodFrom' => "du",
       'PeriodTo' => "au",
       'NumberEntry' => "Nombre d'entrantes",
       'Next day' => "Journée suivante",
       'Previous day' => "Journée précédente"
    )
);

*/

/**
 * Simple function to display
 * some text from an unique string id depending on the
 * LANG value.
 * Returns the associated text, if not found
 * uses english language.
 */
/*function textFromId($s) 
{
  global $messages;
  
  if(isset($messages[LANG][$s])) 
    {
      return $messages[LANG][$s];
    } 
  else if(isset($messages['en_US'][$s]))
    { // if not found true english version
      return $messages['en_US'][$s];
    }
  else
    { // otherwise do no translate
      return $s;
    }
}
*/
?>
