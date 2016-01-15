<?php

/**
 * @class PeriodTitle
 * @author fab
 *
 * Handle period titles
 */
class PeriodTitle
{
  var $cbSteps = array( "RAW" => "cb_RAW",
			"minute" => "cb_step_minute",
			"hour" => "cb_step_hour",
			"day" => "cb_step_day",
			"month" => "cb_step_month" );
  var $step = "minute";
  var $cb = "cb_step_minute";

  function PeriodTitle($nstep)
    {
      $n_step = decomposeNStep($nstep);

      $this->step = $nstep;
      $this->cb = $this->cbSteps[$n_step["step"]];
    }

  function getPeriodTitle($period)
    {
      return call_user_func(array($this, $this->cb), $period);
    }

  function cb_step_minute($period)
    {
      return "de " . strftime("%Hh%M", strtotime($period["start"])) 
	. " à " . strftime("%Hh%M", strtotime($period["end"]));
    }

  function cb_step_hour($period)
    {
      return "de " . strftime("%Hh%M", strtotime($period["start"])) 
	. " à " . strftime("%Hh%M", strtotime($period["end"]));
    }

  function cb_step_day($period)
    {
      return strftime("%A %e", strtotime($period["start"]));
    }

  function cb_step_month($period)
    {
      return strftime("%B", strtotime($period["start"]));
    }

  function cb_RAW($period)
    {
      return strftime("%d/%m/%Y %H:%M:00", strtotime($period["start"]));
    }
}

?>