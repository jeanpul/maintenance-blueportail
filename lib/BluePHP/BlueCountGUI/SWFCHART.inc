<?php

include_once('BluePHP/charts_php/charts.php');


function SWFCHART_object($width, $height, $tmpdatafile)
{
  global $isHTTPS;
  
  $res = InsertChart(SWFCHART_PATH . "charts.swf", 
		     SWFCHART_PATH . "charts_library", 
		     "chartmono.php?uniqueURL=" . time() . "&data=" . $tmpdatafile, 
		     $width, $height, SWFCHART_BGCOLOR, true, SWFCHART_KEY);
  if($isHTTPS)
    {
      return str_ireplace("http://", "https://", $res);
    }
  return $res;   
}

function SWFCHART_savetmpdata($strToGr, $tmpdatafile)
{
  $handle = fopen($tmpdatafile, "w");	  
  fwrite($handle, $strToGr);	  
  fclose($handle);
}

function SWFCHART_chartForFlow($tabularFlowXMLChart, $tmpdatafile)
{
  global $isHTTPS;
 
  $skip = $tabularFlowXMLChart->ncols > 30 ? (int)($tabularFlowXMLChart->ncols / 30) : 0;
  $tabularFlowXMLChart->setRepresentation("xml");
  $strToGr = "<chart>\n";
  $strToGr .= $tabularFlowXMLChart->toString();
  $strToGr .= "<series_gap set_gap='10'/>\n";
  $strToGr .= "<legend_label size='11'/>\n";
  $strToGr .= "<axis_category size='11' skip='$skip' orientation='diagonal_down'/>\n";
  $strToGr .= "<axis_value size='11'/>\n";
  $strToGr .= "<chart_type>column</chart_type>\n";
  
  $strToGr .= "<chart_border color='000000' top_thickness='2' bottom_thickness='2' left_thickness='2' right_thickness='2' />\n";
  $strToGr .= "<chart_grid_h alpha='10' color='000000' thickness='1' type='solid' />\n";
  $strToGr .= "<chart_rect positive_color='ffffff' positive_alpha='80'/>\n";
  $strToGr .= "<chart_value position='cursor' color='000000' background_color='aaff00' alpha='80' />\n";
  $strToGr .= "<series_color><color>768bb3</color></series_color>\n";
  
  $strToGr .= "</chart>\n";
  
  SWFCHART_savetmpdata($strToGr, $tmpdatafile);

  return SWFCHART_object(500, 280, $tmpdatafile); 
  
}

function SWFCHART_chartForMean($tabularMeanXMLChart, $tmpdatafile)
{
  $skip = $tabularMeanXMLChart->ncols > 30 ? (int)($tabularMeanXMLChart->ncols / 30) : 0;
  $tabularMeanXMLChart->setRepresentation("xml");
  $strToGr = "<chart>\n";
  $strToGr .= $tabularMeanXMLChart->toString();
  
  $strToGr .= "<series_gap set_gap='10'/>\n";
  $strToGr .="<series_color>";
  $strToGr .="<color>768bb3</color>";
  //$strToGr .="<color>ffee88</color>";
  $strToGr .="<color>000000</color>";
  $strToGr .="<color>ff6600</color>";
  $strToGr .="</series_color>";
  
  $strToGr .= "<axis_category size='11' skip='$skip' orientation='diagonal_down'/>\n";
  $strToGr .= "<axis_value size='11'/>\n";
  
  $strToGr .= "<chart_type>\n";
  $strToGr .= " <string>column</string>\n";
  $strToGr .= " <string>line</string>\n";
  $strToGr .= " <string>line</string>\n";      
  $strTogr .= "</chart_type>\n";
  
  $strToGr .= "<legend_label size='11'/>\n";
  
  $strToGr .= "<chart_border color='000000' top_thickness='2' bottom_thickness='2' left_thickness='2' right_thickness='2' />\n";
  $strToGr .= "<chart_grid_h alpha='10' color='000000' thickness='1' type='solid' />\n";
  $strToGr .= "<chart_rect positive_color='ffffff' positive_alpha='80'/>\n";
  $strToGr .= "<chart_value position='cursor' color='000000' background_color='aaff00' alpha='80' />\n";
  //$strToGr .= "<series_color><color>768bb3</color></series_color>\n";
  
  $strToGr .= "</chart>\n";
  
  SWFCHART_savetmpdata($strToGr, $tmpdatafile);

  return SWFCHART_object(500, 500, $tmpdatafile);   
}

function SWFCHART_chartForWaitingTime($tabularWaitingTimeXMLChart, $tmpdatafile)
{
  $skip = $tabularWaitingTimeXMLChart->ncols > 30 ? (int)($tabularWaitingTimeXMLChart->ncols / 30) : 0;
  $tabularWaitingTimeXMLChart->setRepresentation("xml");
  $strToGr = "<chart>\n";
  $strToGr .= $tabularWaitingTimeXMLChart->toString();
  $strToGr .= "<legend_label size='14'/>\n";
  $strToGr .= "<axis_category skip='$skip' margin='true' size='11' orientation='diagonal_down'/>\n";
  $strToGr .= "<axis_value size='11'/>\n";
  $strToGr .= "<axis_value_text>\n" .
    "<string></string>\n" .
    "<string></string>\n" .
    "<string></string>\n" .
    "<string></string>\n" .
    "<string></string>\n" .
    "</axis_value_text>\n";
  $strToGr .= "<chart_type>stacked column</chart_type>\n";
  $strToGr .= "<series_gap set_gap='0' bar_gap='0' />\n";
  $strToGr .= "<series_color>\n" .
    "<color>89f600</color>\n" .
    "<color>f68900</color>\n" .
    "<color>ff3300</color>\n" .
    "</series_color>\n";
  $strToGr .= "<chart_grid_h thickness='0'/>\n";
  $strToGr .= "</chart>\n";
  
  
  SWFCHART_savetmpdata($strToGr, $tmpdatafile);
  
  return SWFCHART_object(500, 280, $tmpdatafile);
}


?>