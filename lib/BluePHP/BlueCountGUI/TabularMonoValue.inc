<?php

include_once("BluePHP/Utils/Tabular.inc");

class TabularMonoValue extends Tabular
{
  function TabularMonoValue($titles, $values, $keys)
    {
      $ncols = count($titles);
      $nrows = count($values) + 1; // add title row

      $data = $titles;      
      foreach($values as $k => $v)
	{
	  foreach($keys as $colVal)
	    {
	      $data[] = is_numeric($v[$colVal]) ? round($v[$colVal]) : $v[$colVal];
	    }
	}
      parent::Tabular($data, $nrows, $ncols, true, false);
    }

  function setRepresentation($rep)
    {
      if($rep == "html-table" || $rep == "html-table-utf8")
	{
	  parent::setRepresentation($rep);
	  parent::setSymbol("PREHCELL", "<th id=\"TopTitle\">");
	  for($r = 1; $r < $this->nrows;)
	    {
	      parent::setSymbolOnRow($r, "PRECELL", "<td id=\"cellB\" align=\"right\">");
	      ++$r;
	      parent::setSymbolOnRow($r, "PRECELL", "<td id=\"cellA\" align=\"right\">");
	      ++$r;
	    }
	}
      else
	{
	  parent::setRepresentation($rep);
	}
    }

  function getData($params = array())
    {
      $this->setRepresentation("html-table-utf8");
      return $this->toString();      
    }
  }

?>
