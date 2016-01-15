<?php

/**
 * \file Gnuplot.inc
 * \brief Gnuplot interface to draw graphics into a jpeg
 * \author Fab
 */

include_once("BluePHP/Utils/Tabular.inc");

/**
 * \class Gnuplot
 * \brief Gnuplot interface to draw jpeg graphics
 * 
 * The gnuplot command should be /usr/bin/gnuplot.
 * The user terminal is jpeg.
 * The distinction is made between the plots and the data.
 * - Firstly, create some data using the Tabular class and gives
 * an unique id for each one.
 * - Then select which representation you need that is histogram, line ...
 * - Then, add 2D plots by specifiy the data source and the concerned
 * x and y columns positions.
 * - Finally, send the plot and get the jpeg output file.
 * 
 * \deprecated Never used anymore 
 * \see Tabular
 */
class Gnuplot
{
  /**
   * For each representation, gives a callback methods
   * used to get the right plot command part.
   */
  var $callbacks = array ( "histogram" => "addPlotHistogram",
			   "line" => "addPlotLine" );

  /**
   * jpeg output file name
   */
  var $output;

  /**
   * File description used to communicate
   * with gnuplot
   */
  var $handle;

  /**
   * Plot representation ( 'histogram', 'line', ... )
   */
  var $rep;

  /**
   * An array of array of data indexed by an unique
   * id
   */
  var $plotsData;

  /**
   * An array of parts of plot commands with the
   * data id associated.
   */
  var $plotsCmd;

  /**
   * \brief Create the class and set default parameters
   *
   * By default a 'line' representation is used.
   */
  function Gnuplot($output = "")
    {
      if(!$output)
	{
	  $this->output = "tmp/default.jpeg";
	}
      else
	{
	  $this->output = $output;
	}
      $this->handle = popen("/usr/bin/gnuplot", "w");
      $this->send($this->generate_header());
      $this->rep = "line";
    }

  /**
   * \brief Initialisation of the jpeg terminal
   */ 
  function generate_header()
    {
      $str = "set terminal jpeg tiny font \"/usr/share/fonts/truetype/ttf-dejavu/DejaVuSansMono.ttf\" 8 size 380,240\n";
      $str .= "set output \"" . $this->output . "\"\n";
      //      $str .= "set style line 1 linecolor rgb '#4762c6'\n";
      //      $str .= "set style line 2 linecolor rgb '#d5e1e8'\n";
      return $str;
    }

  /**
   * \brief Change the plot representation 'line', 'histogram', ...
   */
  function setRepresentation($rep)
    {
      $this->rep = $rep;
      $str = "set style histogram clustered gap 1\n";
      $str = "set yrange [0:*]\n";
      $str .= "set style data histogram\n";
      $str .= "set boxwidth 0.89\n";
      $str .= "set style fill solid 1.0 border -1\n";
      $this->send($str);
    }

  /**
   * \brief Set maximum y value
   */
  function setMaxY($maxy)
    {
      $str = "set yrange [0:$maxy]\n";
      $this->send($str);
    }

  /**
   * \brief Change the output file name
   */
  function setOutput($output)
    {
      $this->output = $output;
      $this->send("set output \"" . $this->output . "\"\n");
    }

  /**
   * \brief Rotate the x or y tics labels by an angle in degree
   * \param $id is either 'x' or 'y'
   * \param $angle degree angle between 0 360
   */
  function rotateTics($id, $angle)
    {
      $this->send("set " . $id . "tics rotate by " . $angle . "\n");
    }

  /**
   * \brief Create a unique data source
   * \param id data source unique id
   * \param tabular the data source \see Tabular
   */
  function createPlotData($id, $tabular)
    {
      $tabular->setRepresentation("csv");
      $tabular->setSymbol("SEPCELL", " ");
      $tabular->setSymbol("PREHCELL", "");
      $tabular->setSymbol("POSTHCELL", "");      
      $this->plotsData[$id] = $tabular->toString();
    }

  /**
   * \brief Create a unique data source
   * \param id data sourcer unique id
   * \param string data source in the form
   * row0value0 row0value1 row0value2 ... \n
   * row1value0 row1value1 row1value2 ... \n
   */
  function createDirectPlotData($id, $str)
    {
      $this->plotsData[$id] = $str;
    }

  /**
   * \brief Add a new plot
   * \param idData the data source unique id
   * \param x the x column id in the data source
   * \param y the y column id in the data source
   * \param label the plot label
   */
  function addPlot($idData, $x, $y, $label)
    {
      $idStyle = count($this->plotsCmd) + 1;
      $this->plotsCmd[] = array( $idData, call_user_func(array ( $this, $this->callbacks[$this->rep] ), $idStyle, $x, $y, $label ) );
    }

  function addPlotHistogram($id, $x, $y, $label)
    {
      return "'-' using $y:xticlabels($x) ls $id title '$label'";
    }

  function addPlotLine($id, $x, $y, $label)
    {
      return "'-' using $x:$y with linespoint ls $id title '$label'";
    }

  /**
   * \brief Send the plot command and the data source to gnuplot
   *
   * Then use Gnuplot::getOutput to get the jpeg output filename
   */
  function sendPlot()
    {
      $plotCmd = "plot ";
      $isFirstPlot = true;
      $plotData = "";
      foreach($this->plotsCmd as $value)
	{
	  if($this->plotsData[$value[0]])
	    {
	      if($isFirstPlot)
		{
		  $isFirstPlot = false;
		}
	      else
		{
		  $plotCmd .= ",";
		}
	      $plotCmd .= $value[1];
	      $plotData .= $this->plotsData[$value[0]] . "e\n";
	    }
	}
      $this->send($plotCmd . "\n");
      $this->send($plotData);
      // clear plots
      $this->plotsCmd = array();
    }

  /**
   * \brief Send immediatly string data to gnuplot
   * \param buffer the string sent to gnuplot
   */
  function send($buffer)
    {
      if(fwrite($this->handle, $buffer) == FALSE)
	{
	  echo "Gnuplot::send error: " . "Cannot send data\n";
	  exit;
	}
    }

  /**
   * \brief Get the last jpeg output file name
   */
  function getOutput()
    {
      return $this->output;
    }

  /**
   * \brief Close the communication with gnuplot
   */
  function close()
    {
      pclose($this->handle);
    }
}


?>