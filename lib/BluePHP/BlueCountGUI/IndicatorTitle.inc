<?php

/**
 * @class IndicatorTitle
 * @author fab
 *
 * returns main titles for an indicator
 */
class IndicatorTitle
{
  var $StrPeriod = array ( "minute" => "by minute",
			   "hour" => "by hour",
			   "day" => "by day",
			   "month" => "by month");

  var $StrPeriodMultiple = array ( "minute" => "by %d minutes",
			   "hour" => "by %d hours",
			   "day" => "by %d days",
			   "month" => "by %d months");

  var $StrTimeGranularities = array ( "minute" => "at %Hh of %B %A %d %Y",
				      "hour" => "of %B %A %d %Y",
				      "day" => "of %B %Y",
				      "month" => "of the year %Y" );

  var $step = "minute";
  var $stepfactor = 1;

  var $generalTitle = "";
  var $subTitle = "";
  var $titles = array();

  function IndicatorTitle($nstep)
    {
      $n_step = decomposeNStep($nstep);
      $this->step = $n_step["step"];
      $this->stepfactor = $n_step["factor"];
    }

  function disableGeneralTitle()
  {
    $this->generalTitle = "";
    $this->subTitle = "";
  }

  function setGeneralTitle($indName, $indEntity, $entName, $indTimestamp)
    {      
      $period = "";
      if($this->stepfactor > 1)
	{
	  $period = _($this->StrPeriodMultiple[$this->step]);
	  $period = sprintf($period, $this->stepfactor);
	}
      else
	{
	  $period = _($this->StrPeriod[$this->step]);
	}
   
      $this->generalTitle = $indName . " " 
	. $period . " " 
	. _($indEntity) . " " 
	. $entName . " " 
	. strftime(_($this->StrTimeGranularities[$this->step]), $indTimestamp);
      $this->subTitle = $indName . " " . strftime(_($this->StrTimeGranularities[$this->step]), $indTimestamp);
    }

  function getGeneralTitle()
    {
      return $this->generalTitle;
    }

  function getSubTitle()
    {
      return $this->subTitle;
    }

  function setValuesTitles($titles)
    {
      $this->titles = $titles;
    }

  function getValuesTitles()
    {
      return $this->titles;
    }

  function getValuesTitle($p)
    {
      return $this->titles[$p];
    }
}

?>