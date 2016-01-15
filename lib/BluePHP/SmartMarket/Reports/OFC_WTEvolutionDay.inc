<?php

include_once("SmartMarket/CashProcess.inc");
include_once("OFC/php-ofc-library/open-flash-chart.php");

function OFC_WTEvolutionDay($cash, $titles, $params)
{
  $chart = new open_flash_chart();

  $title = new title( sprintf($titles["graphMain"], $params["titleDate"]) );
  $title->set_style( "{font-size: 20px; color: #A2ACBA; text-align: center;}" );
  
  $line_numb = new line();
  $v = $cash->getMeanValues();
  $line_numb->set_values( $v );
  $line_numb->set_key( $titles["lineKey"], 12 );
  
  if(count($v))
    {
      $area_thresh = new area();
      $area_thresh->set_values( array_fill(0, count($v), $cash->getThreshold()) );
      $area_thresh->set_colour( "#C8727A" );
      $area_thresh->set_key( $titles["areaKey"], 12 );
      $area_thresh->set_tooltip(null);
      $chart->add_element( $area_thresh );
    }
  
  $x_axis = new x_axis();
  $x_axis->set_steps(15);
  
  $x_labels = new x_axis_labels();
  $x_labels->set_steps(60);
  //  $x_labels->set_vertical();
  $x_labels->set_labels( $cash->getTimeValues() );
  $x_axis->set_labels( $x_labels );
  
  $y_axis = new y_axis();
  $y_axis->set_range(0, $cash->getMaxValue() + 1, 1);
  $y_axis->labels = null;
  $y_axis->set_offset( false );
  
  $x_legend = new x_legend( $titles["xlegend"] );
  $x_legend->set_style( '{font-size: 20px; color: #778877}' );
  
  $y_legend = new x_legend( $titles["ylegend"] );
  $y_legend->set_style( '{font-size: 20px; color: #778877}' );
  
  $chart->add_y_axis( $y_axis );
  $chart->set_title( $title );
  $chart->add_element( $line_numb );
  $chart->set_x_axis($x_axis);    
  $chart->set_x_legend($x_legend);
  $chart->set_y_legend($y_legend);
  
  return $chart;
}

?>