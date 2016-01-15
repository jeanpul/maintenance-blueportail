<?php

include_once("BluePHP/Utils/Tabular.inc");
include_once("BluePHP/Utils/Gnuplot.inc");
include_once("BluePHP/Utils/PlotScale.inc");

if(GRAPHIC_KIND == "SWFCHART")
{
  include_once('BluePHP/charts_php/charts.php');
}

class TabularCompFlow extends Tabular
{
  var $yScale = false;
  
  function TabularCompFlow($titles, $objects, $objectKeyName, $values, $valuesComp)
    {
      $this->yScale = new PlotScale();
      $max = 1;
      $data = $titles;
      $total_v0 = 0;
      $total_v1 = 0;
      $total_v0_comp = 0;
      $total_v1_comp = 0;
      foreach($values as $k => $v)
	{
	  $data[] = myhtmlentities($objects[$k][$objectKeyName]);
	  $data[] = $v[0]["value0"];
	  $data[] = $valuesComp[$k][0]["value0"];
	  $data[] = $v[0]["value1"];
	  $data[] = $valuesComp[$k][0]["value1"];

	  $total_v0 += $v[0]["value0"];
	  $total_v1 += $v[0]["value1"];
	  $total_v0_comp += $valuesComp[$k][0]["value0"];
	  $total_v1_comp += $valuesComp[$k][0]["value1"];

	  $max = max($max, $v[0]["value0"], $v[0]["value1"], $valuesComp[$k][0]["value0"], $valuesComp[$k][0]["value1"]);
	}
      
      $this->yScale->setMaximum($max);

      $data[] = _("Total");
      $data[] = $total_v0;
      $data[] = $total_v0_comp;
      $data[] = $total_v1;
      $data[] = $total_v1_comp;

      parent::Tabular($data, count($data) / count($titles), count($titles), true, true);
    }

  function setRepresentation($rep)
    {
      if($rep == "html-table" || $rep == "html-table-utf8")
	{
	  parent::setRepresentation($rep);
	  parent::setSymbol("PREHCELL", "<th id=\"TopTitle\">");
	  for($r = 1; $r < $this->nrows - 1;)
	    {
	      parent::setSymbolOnRow($r, "PRECELL", "<td id=\"cellB\" align=\"right\">");
	      ++$r;
	      parent::setSymbolOnRow($r, "PRECELL", "<td id=\"cellA\" align=\"right\">");
	      ++$r;
	    }
	  parent::setSymbolOnRow($this->nrows - 1, "PRECELL", "<td id=\"cellC\" align=\"right\">");
	}
      else if($rep == "graphic")
	{
	  parent::setRepresentation("csv");
	  parent::setSymbolOnRow(0, "PREROW", "# ");
	  parent::setSymbolOnRow($this->getNRows() - 1, "PREROW", "# ");
	  parent::setSymbol("SEPCELL", " ");
	}
      else
	{
	  parent::setRepresentation($rep);
	}
    }
  
  function getGraphicLink($id, $key, $title1, $col1, $title2, $col2)
    {
      $this->setRepresentation("graphic");
      if(GRAPHIC_KIND == "GNUPLOT")
	{
	  $gnuplot = new Gnuplot("tmp/" . $id . $key . ".jpeg");
	  $gnuplot->setRepresentation("histogram");
	  
	  $maxY = $this->yScale->getScaleValue();
	  if($maxY)
	    {
	      $gnuplot->setMaxY($this->yScale->getScaleValue());
	    }
	  
	  $gnuplot->rotateTics("x", -45);
	  $gnuplot->createDirectPlotData($id, parent::toString());
	  $gnuplot->addPlot($id, 1, $col1, $title1);
	  $gnuplot->addPlot($id, 1, $col2, $title2);
	  $gnuplot->sendPlot();
	  $gnuplot->close();
	  return "getTempImage.php?src=" . $gnuplot->getOutput() . "&key=" . time();
	}
      else if(GRAPHIC_KIND == "SWFCHART")
	{
	  $strToGr = $this->toString();
	  $datafile = "tmp/datagraph" . $id . $key;
	  $handle = fopen($datafile, "w");

	  $str = "#1 " . $title1 . " " . $title2 . "\n";
	  fwrite($handle, $str);
	  $str = "#2 " . $col1 . " " . $col2 . "\n";
	  fwrite($handle, $str);

	  fwrite($handle, $strToGr);
	  fclose($handle);
	  
	  $res = InsertChart(SWFCHART_PATH . "charts.swf", 
			     SWFCHART_PATH . "charts_library", 
			     "chartmono.php?uniqueURL=" . time() . "&data=" . $datafile, 
			     380, 250, SWFCHART_BGCOLOR, false, SWFCHART_KEY);

	  if($isHTTPS)
	    {
	      return str_ireplace("http://", "https://", $res);
	    }
	  return $res;
	}
    }
}

?>