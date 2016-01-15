<?php

include_once('php-ofc-library/open-flash-chart.php');
//include_once('BluePHP/charts_php/charts.php');
include_once('BluePHP/Utils/PlotScale.inc');

function OPENFLASHCHART_insertObject($width, $height, $data)
{  
  $proto = "http";
  if(isset($_SERVER['HTTPS']))
    {
      $proto = "https";
    }

  $serverName = isset($_SERVER["HTTP_X_FORWARDED_SERVER"]) ? $_SERVER["HTTP_X_FORWARDED_SERVER"] : $_SERVER["SERVER_NAME"];

  if($_SERVER["SERVER_PORT"] != "80")
   {
     $serverName .= ":" . $_SERVER["SERVER_PORT"];
   }

  // openflashchart and generated data must be 
  // in the same location to this script
  $location = pathinfo($_SERVER["PHP_SELF"]);
  $baseURL = $proto . "://" . $serverName . $location["dirname"];
  $scriptAdress = $baseURL . "/open-flash-chart.swf?data-file=" . $baseURL . "/" . $data;

  $str = "";
  $str .='<object classid="clsid:d27cdb6e-ae6d-11cf-96b8-444553540000"
        codebase="http://fpdownload.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=8,0,0,0"
        width="'.$width.'" height="'.$height.'" id="graph-2" align="middle">'."\n";
  
  $str .='<param name="allowScriptAccess" value="always" />'."\n";
  $str .='<param name="movie" value="' . $scriptAdress . '" />'."\n";
  $str .='<param name="quality" value="high" />'."\n";
  $str .= '<embed src="' . $scriptAdress . '" 
   quality="high"
   bgcolor="#FFFFFF"
   width="'.$width.'"
   height="'.$height.'"
   name="open-flash-chart"
   align="middle"
   allowScriptAccess="always"
   type="application/x-shockwave-flash"
   pluginspage="http://www.macromedia.com/go/getflashplayer" />';
  $str .= '</object>' . "\n";
 
  return $str;
}

function OPENFLASHCHART_savetmpdata($strToGr, $tmpdatafile)
{
  $handle = fopen($tmpdatafile, "w");	  
  fwrite($handle, $strToGr);	  
  fclose($handle);
}

///////////////////////////
function setColorXAxis(&$axis)
{
  $axis->set_colour('#000000');
  $axis->set_grid_colour('#FFFFFF');
}

function setColorYAxis(&$axis)
{
  $axis->set_colour('#000000');
  $axis->set_grid_colour('#bdbdbd');
}


function OPENFLASHCHART_chartForFlow($tabularFlowXMLChart, $tmpdatafile)
{
  $str = "";

  $chart = new open_flash_chart(); 

  $chart->set_bg_colour('#FFFFFF');

  $title = new title( _("Total") . " " . $tabularFlowXMLChart->getTotal());
  $title->set_style("{font-size: 18px;}");
  $chart->set_title($title);

  $ncols = $tabularFlowXMLChart->getNCols();
  $nrows = $tabularFlowXMLChart->getNRows();
  
  $min = 0;
  $max = 0;

  $y_maxidx = 15;
  $x_maxidx = 24; 

  $labelcolor = "#000000";
  $labelsize = 11;
  $labelrotate = 45;
  {
    $r = 0;
    $tmp = array();
    for($c = 1; $c < $ncols; $c++)
      {	  
	$tmp[] = new x_axis_label($tabularFlowXMLChart->data[$c +$r * $ncols],
				  $labelcolor, $labelsize, $labelrotate);
      } 
    $xaxis = new x_axis();
    $xaxislabels = new x_axis_labels();
    $xaxislabels->set_labels($tmp);
    $step = (int)($ncols / $x_maxidx);
    if($step == 0) $step = 1;
    $xaxislabels->visible_steps($step);
    $xaxis->set_labels($xaxislabels);

    setColorXAxis($xaxis);

    $chart->set_x_axis($xaxis);
  }
  for($r = 1; $r < $nrows; $r++)
    {
      $tmp = array();
      for($c = 1; $c < $ncols; $c++)
	{	  
	  $val = $tabularFlowXMLChart->data[$c +$r * $ncols];
	  $tmp[] = $val;
	  if($val > $max) $max = $val;
	  else if($val < $min) $min = $val;
	}
     

      $bar = new bar();
      $bar->set_key($tabularFlowXMLChart->data[0 +$r * $ncols], 12);
      $bar->set_values($tmp);
      $chart->add_element( $bar );
    }

  
  $ps = new PlotScale();
  $ps->setMaximum($max);
  $max = $ps->getScaleValue();

  $step = 1 + (int)(($max - $min) / $y_maxidx);

  $yaxis = new y_axis();
  $yaxis->set_range($min, $max, $step);  

  setColorYAxis($yaxis);
  $chart->set_y_axis($yaxis);

  $str .= $chart->toString();
  OPENFLASHCHART_savetmpdata($str, $tmpdatafile);
  return OPENFLASHCHART_insertObject("100%", "400px", $tmpdatafile);
}

function OPENFLASHCHART_chartForMean($tabularMeanXMLChart, $tmpdatafile)
{
  $str = "";

  $chart = new open_flash_chart(); 

  $chart->set_bg_colour('#FFFFFF');

  $ncols = $tabularMeanXMLChart->getNCols();
  $nrows = $tabularMeanXMLChart->getNRows();
  
  $min = 0;
  $max = 0;

  $y_maxidx = 15;
  $x_maxidx = 24;

  $labelcolor = "#000000";
  $labelsize = 11;
  $labelrotate = 45;
  
  $tmplabel = array();
  $tmpval = array();
  $tmpmin = array();
  $tmpmax = array();
  for($c = 1; $c < $ncols; $c++)
    {	  
      $tmplabel[] = new x_axis_label($tabularMeanXMLChart->data[$c + 0 * $ncols],
				     $labelcolor, $labelsize, $labelrotate);
      
      $dataval = $tabularMeanXMLChart->data[$c + 1 * $ncols];
      $datamin = $tabularMeanXMLChart->data[$c + 2 * $ncols];
      $datamax = $tabularMeanXMLChart->data[$c + 3 * $ncols];
      
      $tmpval[] = $dataval;
      $tmpmin[] = $datamin;	
      $tmpmax[] = $datamax; 
      
      if($datamax > $max) $max = $datamax;
      if($datamin < $min) $min = $datamin;	
    } 
  
  $xaxis = new x_axis();
  $xaxislabels = new x_axis_labels();
  $xaxislabels->set_labels($tmplabel);
  $step = 1 + (int)($ncols / $x_maxidx);
  $xaxislabels->visible_steps($step);
  $xaxis->set_labels($xaxislabels);
  setColorXAxis($xaxis);
  $chart->set_x_axis($xaxis);

   
  $bar_val = new bar();
  $bar_val->set_key($tabularMeanXMLChart->data[0 + 1 * $ncols], 12);
  $bar_val->set_values($tmpval);
  $bar_val->set_colour("#768bb3");
  //$bar_val->set_alpha(1);
  $chart->add_element( $bar_val );

  $line_min = new line();
  $line_min->set_key($tabularMeanXMLChart->data[0 + 2 * $ncols], 12);
  $line_min->set_values($tmpmin);
  $line_min->set_colour("#000000");
  $chart->add_element( $line_min );  

  $line_max = new line();
  $line_max->set_key($tabularMeanXMLChart->data[0 + 3 * $ncols], 12);
  $line_max->set_values($tmpmax);
  $line_max->set_colour("#ff6600");
  $chart->add_element( $line_max );
  
  $step = 1 + (int)(($max - $min) / $y_maxidx);

  $yaxis = new y_axis();
  $yaxis->set_range($min, $max, $step);
  setColorYAxis($yaxis);
  $chart->set_y_axis($yaxis);

  $str .= $chart->toString();
  OPENFLASHCHART_savetmpdata($str, $tmpdatafile);
  return OPENFLASHCHART_insertObject("100%", "400px", $tmpdatafile);
}


function OPENFLASHCHART_chartForWaitingTime($tabularWaitingTimeXMLChart, 
					    $tmpdatafile)
{ 
  $str = "";
  $ncols = $tabularWaitingTimeXMLChart->getNCols();
  $nrows = $tabularWaitingTimeXMLChart->getNRows();

  $color = array("#89f600", "#f68900", "#ff3300", "#993300");
  $colorsize = count($color);

  $chart = new open_flash_chart();

  $chart->set_bg_colour('#FFFFFF');

  $bar = new bar_stack();
  $bar->set_alpha(1);
  $bar->set_colours($color);
  
  $keys = array();
  for($r = 1; $r < $nrows; $r++)
    {
      $keys[] = new bar_stack_key($color[($r-1) % $colorsize],
				  $tabularWaitingTimeXMLChart->data[$ncols * $r], 13);
    }
  $bar->set_keys($keys);
 
  $labelcolor = "#000000";
  $labelsize = 11;
  $labelrotate = 45;
  $y_maxidx = 15;
  $x_maxidx = 24;

  $max = 0;


  $tmplabel = array();
  for($c = 1; $c < $ncols; $c++)
    {	  
      $tmplabel[] = new x_axis_label($tabularWaitingTimeXMLChart->data[$c],
				     $labelcolor, $labelsize, $labelrotate);
      
    } 

  $xaxis = new x_axis();
  $xaxislabels = new x_axis_labels();
  $xaxislabels->set_labels($tmplabel);
  $step = 1 + (int)($ncols / $x_maxidx);
  $xaxislabels->visible_steps($step);
  $xaxis->set_labels($xaxislabels);
  setColorXAxis($xaxis);
  $chart->set_x_axis($xaxis);
  
  $c = 1; $r =1;
  //$str = " ".$tabularWaitingTimeXMLChart->data[$c + $ncols * $r];
  
  $tmpstack = array(0, 0, 0);
  $tmpdata = array();
  for($c = 1; $c < $ncols; $c++)
    {
      $ts = array();
       for($r = 1; $r < $nrows; $r++)
	 {
	   $val = $r * $tabularWaitingTimeXMLChart->data[$c + $ncols * $r];
	   $ts[] = $val;
	   if($val > $max) $max = $val;
	 }       
      $bar->append_stack($ts);
    } 
  
  $chart->add_element($bar);
  $yaxis = new y_axis();
  $yaxis->set_stroke(0);
  $yaxis->set_label_text("");
  setColorXAxis($yaxis);
  $chart->set_y_axis($yaxis);

  $bar->set_tooltip('#x_label#');//, Value [#val#]<br>Total [#total#]' ); */
  
  $tooltip = new tooltip();  
  $tooltip->set_hover();  
  $chart->set_tooltip($tooltip ); 

  $str .= $chart->toString();
  OPENFLASHCHART_savetmpdata($str, $tmpdatafile);
  return OPENFLASHCHART_insertObject("100%", "400px", $tmpdatafile);
}

?>
