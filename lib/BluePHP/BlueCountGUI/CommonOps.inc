<?php

function isIndicatorEnabled($name)
{
  global $indicatorData;
  return isset($indicatorData[$name]) and ($indicatorData[$name]["value"] == "true");
}

function getAnalysisMenu($indicator)
{
  global $EntitiesAnalysis;

  if(is_array($EntitiesAnalysis) and count($EntitiesAnalysis) > 0)
    {
      $str = "<ul>\n";
      // manages one level sub-menu
      $inSubMenu = false;       
      foreach($EntitiesAnalysis as $k => $entry)
	{
	  if(substr($k, -1, 1) == "/")
	    {
	      // supress '/' caracter to be conform
	      // with W3C id name
	      $k = rtrim($k, "/");
	      if($inSubMenu)
		{ // end of sub menu
		  $str .= "</ul></li>\n";
		}
	      // start a new sub menu
	      $inSubMenu = true;
	      // sub menu title
	      $str .= "<li class=\"Title\"><a href=\"#\" onclick=\"javascript:switchVisible('" . "${k}_" . $indicator->getId() . "');\">" . myhtmlentities(_($entry)) . " :</a></li>\n";
	      $str .= "<li><ul class=\"Section\" id=\"" . "${k}_" . $indicator->getId() . "\">\n";
	    }
	  else if(isIndicatorEnabled($k))
	    {
	      if($k == $indicator->getType())
		{
		  $str .= "<li class=\"Entry\"><u>" . myhtmlentities(_($entry)) . "</u></li>\n";
		}
	      else
		{ // fab : maybe we could manage it directly in the li without a href
		  $str .= "<li class=\"Entry\"><a href=\"#\" onclick=\"javascript:analysisChanged('" . $indicator->getId() . "', '$k');\">" . myhtmlentities(_($entry)) . "</a></li>\n";
		}
	    }
	}
      if($inSubMenu)
	{ // we were at end in a sub menu
	  $str .= "</ul></li>\n";
	}
      $str .= "</ul>\n";
      return $str;
    }
  return "";
}

function getSiteParameters($indicator)
{
  global $configData;

  $str = "<h1>". $configData["SITE_TITLE"]["value"] . "</h1>\n";
  return $str;
}

function getAnalysisParameters($indicator)
{
  $str = "<div class=\"SectionsMenu\">\n"; 
  $str .= "<h3>" . myhtmlentities(_("Indicators list")) ." :</h3>\n";
  $str .= getAnalysisMenu($indicator);
  $str .= "</div>\n";  
  return $str;
}

function getCSVLink($indicator)
{
  $str = "";
  $output = "IndicatorCSV.php?" . $indicator->getURLInputParameters();
  $str .= "<div class=\"Tools\"><a id=\"export\" href=\"$output\">" . myhtmlentities(_("export")) . "</a> " .
    "<a id=\"print\" href=\"#\" onclick=\"javascript:window.print()\" alt=\"print\">" . myhtmlentities(_("print")) . "</a>" . 
    "</div>\n"; 
  return $str;
}

function getGraphicPairTable($l1, $titlel1 = "", $l2 = "", $titlel2 = "")
{
  $str = "<div class=\"DataTable\">\n";
  if($l1)
    {

      if($titlel1 != "")
	{
	  $str .= "<h3>$titlel1</h3>\n";
	}
       if(GRAPHIC_KIND == "SWFCHART" || GRAPHIC_KIND == "OPENFLASHCHART") 
	{
	  $str .= "<span class=\"SWChart\">". $l1 . "</span>\n";
	}
      else 
	{
	  $str .= "<img src=\"$l1\"></img>\n";
	}      
    }
  if($l2)
    {
      if($titlel2 != "")
	{
	  $str .= "<h3>$titlel2</h3>\n";
	}
       if(GRAPHIC_KIND == "SWFCHART" || GRAPHIC_KIND == "OPENFLASHCHART") 
	{
	  $str .= "<span class=\"SWChart\">". $l2 . "</span>\n";
	}
      else 
	{
	  $str .= "<img src=\"$l2\"></img>\n";
	}      
    }
  $str .= "</div>\n";
  return $str;
}

function buildDefaultLeftPart($indicator)
{
  global $configData;

  if($configData["AnalysisSelection"]["value"] == "enabled")
    {
      return getAnalysisParameters($indicator);
    }
  return "";
}

function getTabularBox($indicator, $title, $tabularHTML)
{
  $str = "<h1>" . myhtmlentities($title) . "</h1>\n";
  $str .= getCSVLink($indicator);
  $str .= "<div class=\"DataTable\" id=\"Tabular\">\n";
  $str .= $tabularHTML;
  $str .= "</div>\n";
  return $str;
}

function getGraphicalBox($id, $title, $graphicalHTML)
{
  $str = "<h1>" . myhtmlentities($title) . "</h1>\n";
  $str .= $graphicalHTML;
  return $str;
}

function displayInputParameters($indicator)
{
  $str = "<form action=\"CountingGeneric.php\" method=\"get\" id=\"" . $indicator->getParamsId() . "\">\n";
  $str .= $indicator->getInputParameters();
  $str .= getBluePortailInputs();
  $str .= "</form>\n";
  return $str;
}

function getIndicatorMainPartContents($indicator)
{
  global $configData;

  $str = displayInputParameters($indicator);
  $str .= getSiteParameters($indicator);

  // analysis part is display only if AnalysisSelection is enabled into
  // the user Config parameters
  if($configData["AnalysisSelection"]["value"] == "enabled")
    {
      $str .= "<div class=\"AnalysisMenuPart\" id=\"AnalysisPart_" . $indicator->getId() . "\">\n";
      $str .= buildDefaultLeftPart($indicator);
      $str .= "</div>\n";
    }
  $str .= "<div class=\"DataPart\" id=\"" . $indicator->getDataContentsId() . "\">\n";
  $str .= $indicator->getDataContents();
  $str .= "</div>\n";
  $str .= "<div class=\"ParametersPart\" id=\"ParamsContents_" . $indicator->getId() . "_ParamsContents\">\n";
  $str .= $indicator->getParamsContents();
  $str .= "</div>\n";
  return $str;
}

function translateArrayTerms()
{
  global $Menu;

  // ** TRANSLATION OF TERMS IN ARRAYS AFTER LOADING OF THE USER LANG **
  // Translation of Menu
  foreach(array_keys($Menu) as $k)
    {
      $Menu[$k] = _($Menu[$k]);
    }
}

function getFormErrors($errorMsg)
{
  $errors = FALSE;

  if($errorMsg)
    {
      $errors = "<tr><td width=\"40%\">" . myhtmlentities(_("ErrorMsg")) . "</td>\n";
      $errors .= "<td width=\"60%\"><span class=\"ErrorMsg\">\n";
      foreach($errorMsg as $msg)
	{
	  if($msg == "ErrorMultiple")
	    {
	      // Fab: if we use htmlentities here then we lose
	      // the HTML link in the case of the multiple access error msg
	      global $pageName;
	      $errors .= sprintf(_($msg), $pageName) . "\n";
	    }
	  else
	    {
	      $errors .= myhtmlentities(_($msg)) . "\n";
	    }
	}
      $errors .= "</span></td>\n";
      $errors .= "</tr>\n";
    }

  return $errors;
}

function getFormAuthBox($pageName, $extraInputs ="")
{
  global $errorMsg;

  $auth = "<div class=\"formAuthBox\">\n";
  $auth .= "<h1>" . myhtmlentities(_("BlueCount Manager connection")) . "</h1>\n";

  $auth .= '<form action="' . $pageName . '" method="post">' . "\n";
  $auth .= "<fieldset><legend>" . myhtmlentities(_("Enter")) . "</legend>\n";
  $auth .= "<table>\n";
  // check if there are errors
  $errors = getFormErrors($errorMsg);
  if($errors)
    {
      $auth .= getFormErrors($errorMsg);
    } 
  $auth .= "<tr><td width=\"40%\">" . myhtmlentities(_("Login")) . ':</td><td width="60%"><input type="text" name="login" value="" maxlength="20" ></input></td></tr>' . "\n";
  $auth .= "<tr><td width=\"40%\">" . myhtmlentities(_("Passwd")) . ':</td><td width="40%"><input type="password" name="passwd" value="" maxlength="20" ></input></td></tr>' . "\n";
  $auth .= "<tr><td width=\"40%\"></td><td width=\"60%\"><input name=\"submit\" type=\"submit\" value=\"" . myhtmlentities(_("Connect")) . '" ></input>' . "</td></tr>\n";
  $auth .= "</table>\n";
  // fab : missing language selection
  $auth .= $extraInputs;
  $auth .= "</fieldset>\n";
  $auth .= "</form>\n";
  $auth .= "</div>";

  return $auth;
}

function decomposeNStep($nstep){
  $n = 1;
  $number = "";
  $idx = 0;
  while(ctype_digit($nstep[$idx]))
    {
      $number .= $nstep[$idx];
      $idx++;
    }
  if($number != "") $n = $number;
  $step=substr($nstep, $idx);
  
  return array("step" => $step, "factor" => $n);
}

?>
