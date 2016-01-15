<?php

include_once("OFC/php-ofc-library/open-flash-chart.php");

function HC_QueueEvolutionDay($queue, $titles, $container, $params)
{
  return array( "chart" => array( "renderTo" => $container, 
				  "zoomType" => "xy" ), 
		"title" => array( "text" =>  sprintf($titles["graphMain"], $params["titleDate"]) ),
		"xAxis" => array( "title" => array( "text" => $titles["xlegend"] ),
				  "type" => "datetime",
				  "tickPixelInterval" => 150 ),
		"yAxis" => array( array( "title" => array( "text" => $titles["ylegend"] ),
					 "min" => 0 ),
				  array( "title" => array( "text" => $titles["numbKey"] ),
					 "min" => 0,
					 "opposite" => true ),
				  array( "title" => array( "text" => "Flow" ),
					 "min" => 0,
					 "opposite" => true ) ),
		"legend" => array( "layout" => "vertical",
				   "align" => "left",
				   "verticalAlign" => "top",
				   "x" => 120,
				   "y" => 100,
				   "floating" => true,
				   "backgroundColor" => "#ffffff" ),
		"series" => array( 
				  array( "name" => $titles["wtKey"],
					 "data" => $queue->getWTValues() ),
				  array( "name" => $titles["numbKey"],
					 "yAxis" => 1,
					 "data" => $queue->getNValues() ),
				  array( "name" => "Flow",
					 "yAxis" => 2,
					 "data" => $queue->getFlow() ) )
		);
}

function OFC_QueueEvolutionDay($queue, $titles, $params)
{
  $chart = new open_flash_chart();

  $title = new title( sprintf($titles["graphMain"], $params["titleDate"]) );
  $title->set_style( "{font-size: 20px; color: #A2ACBA; text-align: center;}" );
  
  $line_wt = new line();
  $wt = $queue->getWTValues();
  $line_wt->set_values( $wt );
  $line_wt->set_key( $titles["wtKey"], 12 );
  $line_wt->set_colour( "#6baf2c" );
  //  $line_wt->set_key_on_click("toggle-visibility");

  $line_numb = new line();
  $n = $queue->getNValues();
  $line_numb->set_values( $n );
  $line_numb->set_key( $titles["numbKey"], 12 );
  $line_numb->attach_to_right_y_axis();
  //  $line_numb->set_key_on_click("toggle-visibility");
  
  if(count($wt))
    {
      $area_thresh = new area();
      $area_thresh->set_values( array_fill(0, count($wt), $queue->getThreshold()) );
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
  $x_labels->set_labels( $queue->getTimeValues() );
  $x_axis->set_labels( $x_labels );
  
  $y_axis = new y_axis();
  $y_axis->set_range(0, $queue->getStat("MaxWT") + 1, 1);
  $y_axis->labels = null;
  $y_axis->set_offset( false );

  $y_axis_numb = new y_axis();
  $y_axis_numb->set_range(0, $queue->getStat("MaxN") + 1, 5);
  $y_axis_numb->labels = null;
  $y_axis_numb->set_offset( false );
  
  $x_legend = new x_legend( $titles["xlegend"] );
  $x_legend->set_style( '{font-size: 20px; color: #778877}' );
  
  $y_legend = new x_legend( $titles["ylegend"] );
  $y_legend->set_style( '{font-size: 20px; color: #778877}' );
  
  $chart->add_y_axis( $y_axis );
  $chart->set_title( $title );
  $chart->add_element( $line_wt );
  $chart->add_element( $line_numb );
  $chart->set_x_axis($x_axis);    
  $chart->set_y_axis_right($y_axis_numb);
  $chart->set_x_legend($x_legend);
  $chart->set_y_legend($y_legend);
  
  return $chart;
}

?>