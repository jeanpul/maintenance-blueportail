<?php

include_once("BluePHP/Utils/Tabular.inc");

class TabularBi extends Tabular
{
  function TabularBi($data, $nrows, $ncols, $haveHRow = false, $haveHCell = false)
  {
    parent::Tabular($data, $nrows, $ncols, $haveHRow, $haveHCell);
  }

  function setRepresentation($rep)
    {
      if($rep == "html-table")
	{
	  parent::setRepresentation($rep);
	  for($r = 1; $r < $this->nrows;)
	    {
	      parent::setSymbolOnRow($r, "PRECELL", "<td class=\"cellB\" align=\"right\">");
	      ++$r;
	      parent::setSymbolOnRow($r, "PRECELL", "<td class=\"cellA\" align=\"right\">");
	      ++$r;
	    }
	}
      else
	{
	  parent::setRepresentation($rep);
	}
    }
}

?>