<?php

/**
 * \class Tabular
 * \brief A set of data with rows and cells
 *
 * A tabular represents a set of data
 * with the following grammar :
 * 
 * - table    := PRETABLE rowlist POSTTABLE
 * - rowlist  := row |\n
 *               rowlist row
 * - row      := PREROW celllist POSTROW
 * - celllist  := hcell |\n
 *               cell |\n
 *               celllist SEPCELL cell
 * - cell     := PRECELL VAL POSTCELL
 * - hcell    := PREHCELL VAL POSTHCELL
 *
 * A tabular is a list of rows
 * Each row is a list of cells
 * A row could contains some hcell (header cell) to put some title
 * This could be used to generate HTML or CSV output.
 */
class Tabular
{
  /**
   * \brief Contains all
   * symbols definition.
   * By default use a CSV representation
   */
  var $symbols = array("PRETABLE" => "",
		       "POSTTABLE" => "",
		       "PREROW" => "",
		       "POSTROW" => "\n",
		       "PREHEADER" => "",
		       "POSTHEADER" => "",
		       "SEPCELL" => ",",
		       "PRECELL" => "",
		       "POSTCELL" => "",
		       "PREHCELL" => "\"",
		       "POSTHCELL" => "\"",
		       "PROCESSSTR" => "dummy");

  /**
   * current representation kind
   */
  var $representation = "csv";

  /**
   * \brief Contains some particular symbols
   * definition for the rows
   */
  var $rowSymbols = array();

  /**
   * \brief Contains all the cell values
   */
  var $data;

  /**
   * \brief Represents the number of rows
   */
  var $nrows;

  /**
   * \brief Represents the number of columns
   */
  var $ncols;

  /**
   * \brief Represents the value used to move from one row
   * to another \see c++ STL stride
   *
   * By default this value is the number of columns
   */
  var $srows;

  /**
   * \brief Represents the value used to move from one column
   * to another \see c++ STL stride
   *
   * By default this value is 1
   */
  var $scols;

  /**
   * \brief Whether or not the first row will be the columns title
   */
  var $haveHRow;

  /**
   * \brief Whether or not the first value of each row will be the row title
   */
  var $haveHCell;

  /**
   * \brief Create the tabular object
   * 
   * This is a 2D array where the data are stored into a 1D array
   * \param $data The tabular cells in a form of a 1D array
   * \param $nrows the number of rows 
   * \param $ncols the number of column
   * \param $haveHRow means that the first row will be the columns titles
   * \param $haveHCell means that the first value of each row will be the row title
   */
  function Tabular($data, $nrows, $ncols, $haveHRow = false, $haveHCell = false)
    {
      $this->data = $data;
      $this->haveHRow = $haveHRow;
      $this->haveHCell = $haveHCell;

      $this->nrows = $nrows;
      $this->ncols = $ncols;
      $this->srows = $this->ncols;
      $this->scols = 1;
    }

  /**
   * for CSV : no processing on str
   * method identity on str
   */
  function dummy($str)
    {
      return $str;
    }

  /**
   * for html : apply htmlentities on str
   */
  function processForHtmlStr($str)
    {      
      return htmlentities($str);
    }

  /**
   * \brief Returns a string representation of a single tabular row
   * \param $row The tabular row used
   * \param $lastHCellIndex cells between [0:$lastHCellIndex[ will be used as title
   */
  function toStringOneRow($r, $lastHCellIndex, $symbols)
    {
      $irow = $r * $this->srows;
      $str = $symbols["PREROW"];
      for($i = 0; $i < $this->ncols; ++$i)
	{ 
	  $icol = $i * $this->scols;
	  if($i < $lastHCellIndex)
	    { // collist := hcell
	      $str .=  $symbols["PREHCELL"] . $this->$symbols["PROCESSSTR"]($this->data[$irow + $icol]) . $symbols["POSTHCELL"];
	    }
	  else
	    {
	      // collist := cell
	      $str .= $symbols["PRECELL"] . $this->data[$irow + $icol] . $symbols["POSTCELL"];
	    }
	  
	  if($i < $this->ncols - 1)
	    { // collist := collist SEPCELL cell
	      $str .= $symbols["SEPCELL"];
	    }
	}
      $str .= $symbols["POSTROW"];
      return $str;
    }

  /**
   * \brief Returns a string representation of the tabular
   */
  function toString()
    {      
      $str = $this->symbols["PRETABLE"];

      for($i = 0; $i < $this->nrows; ++$i)
	{
	  $lastHCellIndex = 0;
	  if($i == 0 and $this->haveHRow)
	    {
	      $str .= $this->symbols["PREHEADER"];
	      $lastHCellIndex = $this->ncols;	      
	    }
	  else if($this->haveHCell)
	    {
	      $lastHCellIndex = 1;
	    }
	  if(isset($this->rowSymbols[$i]))
	    {
	      $str .= $this->toStringOneRow($i, $lastHCellIndex, array_merge($this->symbols, $this->rowSymbols[$i]));
	    }
	  else
	    {
	      $str .= $this->toStringOneRow($i, $lastHCellIndex, $this->symbols); 
	    }
	  if($i == 0 and $this->haveHRow)
	    {
	      $str .= $this->symbols["POSTHEADER"];
	    }
	}
      $str .= $this->symbols["POSTTABLE"];
      return $str;
    }

  /**
   * \brief Returns the value of a column in the 
   * first row
   * \param $col the column position
   */
  function getFirstRowValue($col)
    {
      return $this->data[$col * $this->scols];
    }

  /**
   * \brief Returns the value of a column in the
   * last row
   * \param $col the column position
   */
  function getLastRowValue($col)
    {
      return $this->data[$col * $this->scols + ($this->nrows - 1) * $this->srows];
    }

  /**
   * \brief Changes the ouput string representation
   * \param $rep Could be either 'csv' or 'html:table'
   */
  function setRepresentation($rep)
    {
      if($rep == "csv")
	{
	  $this->symbols = array("PRETABLE" => "",
				 "POSTTABLE" => "",
				 "PREROW" => "",
				 "POSTROW" => "\n",
				 "PREHEADER" => "",
				 "POSTHEADER" => "",
				 "SEPCELL" => ";",
				 "PRECELL" => "",
				 "POSTCELL" => "",
				 "PREHCELL" => "\"",
				 "POSTHCELL" => "\"",
				 "PROCESSSTR" => "dummy");
	  $this->rowSymbols = array ();
	  $this->representation = "csv";
	}
      else if($rep == "html-table" || $rep == "html-table-utf8")
	{
	  $this->symbols = array("PRETABLE" => "<table>\n",
				 "POSTTABLE" => "</table>\n",
				 "PREROW" => "<tr>",
				 "POSTROW" => "</tr>\n",
				 "PREHEADER" => "<thead>\n",
				 "POSTHEADER" => "</thead>\n",
				 "SEPCELL" => "",
				 "PRECELL" => "<td>",
				 "POSTCELL" => "</td>",
				 "PREHCELL" => "<th>",
				 "POSTHCELL" => "</th>",
				 "PROCESSSTR" => $rep == "html-table" ? "processForHtmlStr" :
				 "dummy" /*in case of utf8 no action on the string*/);
	  $this->rowSymbols = array ();
	  $this->representation = $rep;
	}  
    }

  /**
   * \brief Changes the value of a symbol
   * \param $symbol the symbol name \see Tabular::$symbols
   * \param $value the new symbol value
   */
  function setSymbol($symbol, $value)
    {
      $this->symbols[$symbol] = $value;
    }

  /**
   * \brief Changes the value of a symbol for a particular row
   * \param $row the row position
   * \param $symbol the concerned symbol
   * \param $value the new symbol value
   */
  function setSymbolOnRow($row, $symbol, $value)
    {
      if(isset($this->rowSymbols[$row]))
	{
	  $this->rowSymbols[$row][$symbol] = $value;
	}
      else
	{
	  $this->rowSymbols[$row] = array( $symbol => $value );
	}
    }

  /**
   * \brief Returns the value of a symbol
   * \param $symbol the symbol name \see Tabular::$symbols
   */
  function getSymbol($symbol)
    {
      return $this->symbols[$symbol];
    }

  /**
   * \brief Returns the number of rows
   * \return The current number of rows
   */
  function getNRows()
    {
      return $this->nrows;
    }

  /**
   * \brief Returns the number of columns
   * \return The current number of columns
   */
  function getNCols()
    {
      return $this->ncols;
    }
}

?>