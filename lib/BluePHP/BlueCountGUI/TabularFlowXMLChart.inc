<?php

include_once("BluePHP/Utils/Tabular.inc");
include_once("BluePHP/Utils/Gnuplot.inc");
include_once("BluePHP/Utils/PlotScale.inc");

include_once(GRAPHIC_KIND. '.inc');


class TabularFlowXMLChart extends Tabular
{
  function TabularFlowXMLChart($titles, $values, $keys)      
    {
      $this->total = 0;
      // support for multiples categories
      $ncols = count($values) + 1; // add title column
      $nrows = count($keys);
     
      // first col are titles
      for($i = 0; $i < count($titles); $i++)
	{
	  $data[$i*$ncols] = $titles[$i];
	}

      for($c = 1; $c < $ncols; $c++)
	{
	  for($r = 0; $r < $nrows; $r++)
	    {
	      $v = $values[$c - 1][$keys[$r]];
	      $data[$c + $r*$ncols] = is_numeric($v) ? round($v) : $v;
	      $this->total += is_numeric($v) ? round($v) : 0;
	    }
	}
      parent::Tabular($data, $nrows, $ncols, true, true);
    }

  function getTotal()
  {
    return $this->total;
  }

  function setRepresentation($rep)
    {
      if($rep == "xml")
	{
	  parent::setSymbol("PRETABLE", "<chart_data>\n");
	  parent::setSymbol("POSTTABLE", "</chart_data>\n");
	  parent::setSymbol("PREROW", "<row>\n");
	  parent::setSymbol("POSTROW", "</row>\n");
	  parent::setSymbol("PRECELL", "<number>");
	  parent::setSymbol("POSTCELL", "</number>\n");
	  parent::setSymbol("PREHCELL", "<string>");
	  parent::setSymbol("POSTHCELL", "</string>\n");
	  parent::setSymbol("SEPCELL", "");
	}
      else
	{
	  parent::setRepresentation($rep);
	}
    }

  // directly send XML data to chartmono.php
  function getData($params)
    {
      $datafile = "tmp/datagraph" . $params["id"] . $params["key"];
      $method = GRAPHIC_KIND . '_chartForFlow';
      return $method($this, $datafile);
    }
}

?>
