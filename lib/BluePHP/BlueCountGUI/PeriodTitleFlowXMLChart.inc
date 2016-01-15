<?php

include_once("PeriodTitle.inc");

/**
 * @class PeriodTitle
 * @author fab
 *
 * Handle period titles
 */
class PeriodTitleFlowXMLChart
{
  var $cbSteps = array(
			"minute" => "cb_step_minute",
			"hour" => "cb_step_hour",
			"day" => "cb_step_day",
			"month" => "cb_step_month" );
  var $step = "minute";
  var $cb = "cb_step_minute";

  function PeriodTitleFlowXMLChart($nstep)
    {
      $n_step=decomposeNStep($nstep);

      $this->step = $nstep;
      $this->cb = $this->cbSteps[$n_step["step"]];
    }

  function getPeriodTitle($period)
    {
      return call_user_func(array($this, $this->cb), $period);
    }

  function cb_step_minute($period)
    {
      return strftime("%Hh%M", strtotime($period["start"]));
    }

  function cb_step_hour($period)
    {
      return $period["period"];
    }

  function cb_step_day($period)
    {
      return strftime("%e %b", strtotime($period["start"]));
    }

  function cb_step_month($period)
    {
      return strftime("%B", strtotime($period["start"]));
    }
}

?>