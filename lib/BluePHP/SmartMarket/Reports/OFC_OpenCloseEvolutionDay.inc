<?php

include_once("SmartMarket/CashProcess.inc");
include_once("OFC/php-ofc-library/open-flash-chart.php");

function OFC_OpenCloseEvolutionDay($cash, $titles, $params)
{
  $title = new title( sprintf($titles["graphMain"], $params["titleDate" ]) );
  $title->set_style( "{font-size: 20px; color: #A2ACBA; text-align: center;}" );
  
  //  $line_flow = new line();
  //  $line_flow->set_values( $cash->getSmFlowInValues() );
  //  $line_flow->set_key( _("Entrées par minute"), 12 );
  //  $line_flow->set_colour( "#000000" );
  //  $line_flow->attach_to_right_y_axis();

  $stack = new bar_stack();
  $stack->set_colours( array( "#ff0000", "#00ff00", "#0000ff" ) );
  $stack->set_alpha( 0.6 );
  $stack->set_keys( array( new bar_stack_key( "#ff0000", $titles["lineKeyAlert"], 12 ),
			   new bar_stack_key( "#00ff00", $titles["lineKeyActivity"], 12 ),
			   new bar_stack_key( "#0000ff", $titles["lineKeyInactivity"], 12 ) ));

  $a1 = $cash->getFullValues();
  $a2 = $cash->getOpenValues();
  $a3 = $cash->getCloseValues();
  for($i = 0; $i < count($a1); ++$i)
    {
      $stack->append_stack( array( $a1[$i], $a2[$i] - $a1[$i], $a3[$i] ) );
    }  

//  $line_ratio = new line();
//  $line_ratio->set_values( $cash->getRatioValues() );
//  $line_ratio->set_key( _("Ratio"), 12 );
//  $line_ratio->set_colour( "#ff" );
//
//  $line2 = new line();
//  $line2->set_values( $cash->getOpenValues() );
//  $line2->set_colour( "#00ff00" );
  

//  $line_total = new line();
//  $line_total->set_values( $cash->getTotalValues() );
//  $line_total->set_colour( "#ff0000" );
//  $line_total->set_key( _("Total"), 12 );

//  $bar_open = new bar();
//  $bar_open->set_values( $cash->getOpenValues() );
//  $bar_open->set_key( ("Caisses en activité"), 12 );
//
//  $bar_close = new bar();
//  $bar_close->set_values( $cash->getCloseValues() );
//  $bar_close->set_key( ("Caisses vides"), 12 );

  $x_axis = new x_axis();
  $x_axis->set_steps(15);
  
  $x_labels = new x_axis_labels();
  $x_labels->set_steps(60);
  //  $x_labels->set_vertical();
  $x_labels->set_labels( $cash->getTimeValues() );
  $x_axis->set_labels( $x_labels );
  
  $y_axis = new y_axis();
  $y_axis->set_range(0, $cash->getNbCashes(), 5);
  $y_axis->labels = null;
  $y_axis->set_offset( false );
  
  //  $y_axis_r = new y_axis_right();
  //  $y_axis_r->set_range(0, $cash->getMaxFlowIn() + 1, 5);
  //  $y_axis_r->set_range(0, 250, 50);
  //  $y_axis_r->labels = null;
  //  $y_axis_r->set_offset( false );

  $x_legend = new x_legend( $titles["xlegend"] );
  $x_legend->set_style( '{font-size: 20px; color: #778877}' );
  
  $y_legend = new x_legend( $titles["ylegend"] );
  $y_legend->set_style( '{font-size: 20px; color: #778877}' );
  
  $chart = new open_flash_chart();
  $chart->add_y_axis( $y_axis );
  $chart->set_title( $title );
  $chart->add_element( $stack );
  //  $chart->add_element( $line_ratio );
  //  $chart->add_element( $line2 );
  //  $chart->add_element( $line_total );
  //  $chart->add_element( $bar_open );
  //  $chart->add_element( $bar_close );
  $chart->set_x_axis($x_axis);    
  //  $chart->set_y_axis_right($y_axis_r);
  $chart->set_x_legend($x_legend);
  $chart->set_y_legend($y_legend);
  
  return $chart;
}

?>
