<?php

define('GNUPLOT', "/usr/local/bin/gnuplot");

/**
 * \class Histogram
 * \deprecated no more used
 */
class Histogram
{
    /**
     *  title of the graph
     */
    var $title = "HELLO WORLD";
    
    /**
     * min - max on the y-axis
     * if autoY is TRUE, the value along the y-axis are set to the
     * most appropriate value according to the displaying data
     */   
    var $autoY = TRUE;
    var $minY = "0.0";
    var $maxY = "100.0";

    /**
     * array of name to describe x-axis
     * each name is associate to 0, 1 ...
     */   
    var $legendList = array();

    /**
     * array of array
     * each array contain a couple composed of a file name and a
     * column id, The filename can be empty : then the data file used
     * is the previously declared file. the column id is the column of
     * the file where extract the data.
     */
    var $data = array();

    function Histogram()
    {
    }

    function buildConfiguration($file)
    {
        $str  = "set terminal jpeg\n";
	$str .= "set boxwidth 0.9 absolute\n";
	$str .= "set style fill solid 1.00 border -1\n";
	$str .= "set style histogram clustered gap 1 title  offset character 0, 0, 0\n";
	$str .= "set datafile missing '-'\n";
	$str .= "set style data histograms\n";
	$str .= "set title \"" . $this->title . "\"\n";
	$str .= "set xtics border in scale 1,0.5 nomirror rotate by 0 offset character 0, 0.5, 0\n";

	$xtics = "";
	$i = 0;
	foreach($this->legendList as $l)
          {
	     if($xtics != "")
               {
                 $xtics .= ", ";
               }
	     $xtics .= "\"$l\" $i";
	     $i += 1;
          }
	if ($xtics != "")
          {
	     $str .= "set xtics ($xtics)\n";
          }

	if(! $this->autoY) $str .= "set yrange [ $this->minY : $this->maxY ] noreverse nowriteback\n";

        $nb = count($this->data);
        if ($nb > 0)
          {
             $v = $this->data[0];
	     $plotstr = "plot '$v[0]' using 0:xtic(1) ti col, '' u $v[1] ti col";
             for($i = 1; $i < $nb; $i++)
              {
                $v = $this->data[$i];
	        $plotstr .= ", '$v[0]' u $v[1] ti col";
              }
             $str .= "$plotstr\n";
           }

        $handle = fopen("$file", 'w');
        fwrite($handle, $str);
        fclose($handle);
    } 

    function buildJpegImg($file = "")
    {
        $t = gettimeofday();
	$tmpfile = "/tmp/gnuplot" . $t['sec'] . "_" . $t['usec'];

        $this->buildConfiguration($tmpfile);

	$cmd = GNUPLOT." $tmpfile";

	if($file != "")
        {
          $cmd .= " > $file";
          exec($cmd);
          readfile($file);
        }
        else
        {
          passthru($cmd);
        }

	unlink($tmpfile);
    }

    function renderJpegImg($file = "")
    {
      $existingFile = FALSE;
      if($file != "")
      {     
	$existingFile = file_exists($file);
      }
      
      if($existingFile)
      {
        readfile($file);
      }      
      else
      {
        $this->buildJpegImg($file);
      }
    }

    function render($file = "")
    {
        Header("Content-type: image/jpeg");
        $this->renderJpegImg($file);
    }
}

?>