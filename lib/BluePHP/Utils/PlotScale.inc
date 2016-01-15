<?php

class PlotScale
{
  var $yScale = array ( 10, 10, 10, 10, 20, 50, 100, 150, 300, 600, 1200, 2500, 5000, 10000, 20000, 50000, 100000, 200000 );  
  var $max = 0;
  var $maxIdx = -1;

  function PlotScale()
    {
      $this->max = 0;
    }

  function setMaximum($value)
    {
      $this->max = $value;
    }

  function getScaleValue()
    {
      if($this->max > 0)
	{
	  $idx = ceil(log($this->max, 2));
	  if(isset($this->yScale[$idx]))
	    {
	      return $this->yScale[$idx];
	    }
	}
      return $this->max;
    }
}

?>