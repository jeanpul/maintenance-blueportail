<?php

include_once("SmartMarket/CashProcess.inc");
include_once("OFC/php-ofc-library/open-flash-chart.php");

function OFC_FlowInEvolutionDay($cash, $titles, $params)
{
  $title = new title( sprintf($titles["graphMain"], $params["titleDate" ]) );
  $title->set_style( "{font-size: 20px; color: #A2ACBA; text-align: center;}" );
  
  $line_flow = new line();
  $line_flow->set_values( $cash->getSmFlowInValues() );
  $line_flow->set_key( $titles["lineKey"], 12 );
  $line_flow->set_colour( "#000000" );
  $x_axis = new x_axis();
  $x_axis->set_steps(15);
  
  $x_labels = new x_axis_labels();
  $x_labels->set_steps(60);
  //  $x_labels->set_vertical();
  $x_labels->set_labels( $cash->getTimeValues() );
  $x_axis->set_labels( $x_labels );
  
  $y_axis = new y_axis();
  $y_axis->set_range(0, $cash->getMaxFlowIn() + 1, 5);
  $y_axis->labels = null;
  $y_axis->set_offset( false );
  
  $x_legend = new x_legend( $titles["xlegend"]);
  $x_legend->set_style( '{font-size: 20px; color: #778877}' );
  
  $y_legend = new x_legend( $titles["ylegend"]);
  $y_legend->set_style( '{font-size: 20px; color: #778877}' );
  
  $chart = new open_flash_chart();
  $chart->add_y_axis( $y_axis );
  $chart->set_title( $title );
  $chart->add_element( $line_flow );
  $chart->set_x_axis($x_axis);    
  $chart->set_x_legend($x_legend);
  $chart->set_y_legend($y_legend);
  
  return $chart;
}

?>
