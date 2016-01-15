<?php

function HC_QueueEvolutionDay($queue, $titles, $container, $params)
{
  $fillColor = array( "linearGradient" => array( 0, 0, 0, 300 ),
		      "stops" => array( array(0, "Highcharts.theme.colors[0]" ),
					array(1, "rgba(2,0,0,0)" ))
		      );
  $marker = array( "enabled" => false,
		   "states" => array( "hover" => array( "enabled" => true,
							"radius" => 5 )));

  return array( "chart" => array( "renderTo" => $container, 
				  "zoomType" => "x" ), 
		"credits" => array( "enabled" => false ),
		"title" => array( "text" =>  sprintf($titles["graphMain"], $params["titleDate"]) ),
		"xAxis" => array( "title" => array( "text" => $titles["xlegend"] ),
				  "type" => "line",
				  "categories" => $queue->getTimeValues(),
				  "tickInterval" => 60 ),
		"yAxis" => array( array( "title" => array( "text" => $titles["ylegend"] ),
					 "min" => 0,
					 "plotBands" => array( array( "color" => 'rgba(0, 240, 255, .1)',
								      "from" => 0,
								      "to" => $params["alarms"]["WT"] ),
							       array( "color" => 'rgba(255, 0, 0, .1)',
								      "from" => $params["alarms"]["WT"],
								      "to" => $params["alarms"]["WT"] + 100 ) ),
					 ),
				  array( "title" => array( "text" => $titles["numbKey"] ),
					 "min" => 0,
					 "opposite" => true ),
				  array( "title" => array( "text" => $titles["flowKey"] ),
					 "min" => 0,
					 "opposite" => true ) ),
		"legend" => array( "layout" => "vertical",
				   "align" => "left",
				   "verticalAlign" => "top",
				   "x" => 120,
				   "y" => 5,
				   "floating" => true,
				   "backgroundColor" => "#ffffff" ),
		"tooltip" => array( "shared" => true,
				    "crosshairs" => true ),
		"plotOptions" => array( "area" => array( //"fillColor" => $fillColor,
							 "lineWidth" => 1,
							 "shadow" => false,
							 "marker" => $marker 
							 ),
					"line" => array( "lineWidth" => 1, 
							 "marker" => $marker
							 ) ),
		"series" => array(
				  array( "type" => "area", 
					"name" => $titles["wtKey"],
					 "data" => $queue->getWTValues(),
					 "zIndex" => 2 ),
				  array( "type" => "area", 
					"name" => $titles["rwtKey"],
					 "data" => $queue->getRWTValues(),
					 "zIndex" => 3 ),
				  array( "type" => "area",
					 "name" => $titles["numbKey"],
					 "yAxis" => 1,
					 "data" => $queue->getNValues(),
					 "zIndex" => 1 ),
				  array( "type" => "area",
					 "name" => $titles["flowKey"],
					 "yAxis" => 2,
					 "data" => $queue->getFlow(),
					 "zIndex" => 4 )
				  )
		);
}

?>