<?php

include_once("BluePHP/SmartMarket/CashProcess.inc");
include_once("OFC/php-ofc-library/open-flash-chart.php");

function OFC_Shape_Alarm($left, $right, $color)
{
  $today = new shape( $color );
  $today->append_value( new shape_point($left, 0 ) );
  $today->append_value( new shape_point($left, 1 ) );
  $today->append_value( new shape_point($right, 1 ) );
  $today->append_value( new shape_point($right, 0 ) );
  return $today;
}

function OFC_AlarmEvolutionDay($cash, $titles, $params)
{
  $chart = new open_flash_chart();

  $title = new title( sprintf($titles["graphMain"], $params["titleDate"]) );
  $title->set_style( "{font-size: 20px; color: #A2ACBA; text-align: center;}" );

  $v = $cash->getTfValues();
  $inAlarm = false;
  $alarmType = 0; 
  $alarmTypeColors = array( '#FA6900', '#FF0000' );
  $left = 0;
  for($i = 0; $i < count($v); ++$i)
    {
      if($v[$i] >= $params["alarms"]["RZ1"]) 
	{
	  if($v[$i] >= $params["alarms"]["RZ2"])
	    {
	      $alarmType = 1;
	    }
	  if(!$inAlarm)
	    {
	      $left = $i - $params["alarms"]["RZ1"];
	      $inAlarm = true;
	      $alarmType = 0;
	    }
	}
      else if($inAlarm)
	{
	  $inAlarm = false;
	  $chart->add_element(OFC_Shape_Alarm($left, $i - 1, $alarmTypeColors[$alarmType]));	  
	}
    }

  $x_axis = new x_axis();
  $x_axis->set_steps(15);
  
  $x_labels = new x_axis_labels();
  $x_labels->set_steps(60);
  //  $x_labels->set_vertical();
  $x_labels->set_labels( $cash->getTimeValues() );
  $x_axis->set_labels( $x_labels );
  
  $y = new y_axis();
  $y->set_range(0, 1, 1);
  $y->labels = null;
  $y->set_offset( false );
  
  $x_legend = new x_legend( $titles["xlegend"] );
  $x_legend->set_style( '{font-size: 20px; color: #778877}' );
  
  $chart->add_y_axis( $y );
  $chart->set_title( $title );
  $chart->set_x_axis($x_axis);    
  $chart->set_x_legend($x_legend);
  
  return $chart;
}

?>
