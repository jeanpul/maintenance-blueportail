<?php

include_once("BluePHP/Utils/Input.inc");

function getGlobalStatistics($table, $titles, $withActivity = true)
{
  $title = $titles["main"];

  $show = _("show");
  $hide = _("hide");

  $str = <<<EOT
<div id="indicator">
<div id="top-bar">
<h2 class="align-left">$title</h2>
<a href="#" id="hide-button" class="align-right" onClick="HideShowElt(this, 'topTableFrame', '$show', '$hide');">$hide</a>
</div>
<div id="topTableFrame">
<table id="topTable" border="0" cellpading="0" cellspacing="0" class="display">
<thead>
EOT;

  $columnTitlesEnabled = null;
  if($withActivity)
    {
      $columnTitlesEnabled = array( $titles["cash"],
				    $titles["pmean"],
				    $titles["pmax"],
				    $titles["nmax"],
				    $titles["%thresh"],
				    $titles["amean"],
				    $titles["%mean"],
				    $titles["alert1"],
				    $titles["alert2"],
				    $titles["alertmax"] );
    }
  else
    {
      $columnTitlesEnabled = array( $titles["cash"],
				    $titles["pmean"],
				    $titles["pmax"],
				    $titles["nmax"],
				    $titles["%thresh"],
				    $titles["alert1"],
				    $titles["alert2"],
				    $titles["alertmax"] );
    }
  $str .= createTableHeaderRow($columnTitlesEnabled);
  $str .= "</thead><tbody>\n";
  foreach($table as $k => $v)
    {
      if($withActivity)
	{
	  $str .= createTableRow( array( $v["id"],
					 round($v["value"], 1),
					 round($v["MaxWT"], 1),
					 (int) $v["MaxN"],
				     (int) $v["thresholdRate"],
					 $v["activity"],
					 round($v["arate"], 0),
					 $v["alarmDM"],
					 $v["alarmDR"],
					 $v["maxTf"] ) );
	}
      else
	{
	  $str .= createTableRow( array( $v["id"],
					 round($v["value"], 1),
					 round($v["MaxWT"], 1),
					 (int) $v["MaxN"],
					 (int) $v["thresholdRate"],
					 $v["alarmDM"],
					 $v["alarmDR"],
					 $v["maxTf"] ) );
	}
    }

  $str .= "</tbody></table></div></div>\n";
  return $str;
}

function getSummaryTable($data, $titles)
{
  $title = $titles["main"];
  $show = _("show");
  $hide = _("hide");

  $str = <<<EOT
<div id="indicator">
<div id="top-bar">
<h2 class="align-left">$title</h2>
<a href="#" id="hide-button" class="align-right" onClick="HideShowElt(this, 'summaryTableFrame', '$show', '$hide');">$hide</a>
</div>
<div id="summaryTableFrame">
<table id="summaryTable" border="0" cellpading="0" cellspacing="0" class="display">
<thead>
EOT;

  $str .= createTableHeaderRow( array( $titles["pmean"],
				       $titles["pmax"],
				       $titles["nmax"],
				       $titles["%thresh"],
				       $titles["amean"],
				       $titles["%mean"],
				       $titles["alert1"],
				       $titles["alert2"],
				       $titles["alertmax"] ) );
  
  $str .= "</thead><tbody>\n";
  $str .= createTableRow( array( round($data->getTotalMeanValue(), 1),
				 round($data->getMaxValue(), 1),
				 $data->getMaxN(),
				 (int) $data->getThresholdRate(),
				 $data->getTotalMeanActivity(),
				 $data->getActivityRate(),
				 $data->getNbAlarmDM(),
				 $data->getNbAlarmDR(),
				 $data->getMaxTf() ) );
  $str .= "</tbody></table>\n";
  $str .= "<ul class='footnote'><li>" . $titles["pmean_foot"] . "</li>\n" . 
    "<li>" . $titles["amean_foot"] . "</li>\n" . 
    "<li>" . $titles["%mean_foot"] . "</li>\n" . 
    "<li>" . $titles["alert1_foot"] . "</li>\n" . 
    "<li>" . $titles["alert2_foot"] .  "</li>\n" . 
    "<li class='clearFoot'></li>\n" .
    "</ul></div></div>\n";
  return $str;
}

function getOFCIndicator($title, $id, $titleId = false)
{
  $str = '<div id="indicator"><div id="top-bar">' . 
    '<h2 class="align-left"';
  if($titleId)
    {
      $str .= " id='$titleId'";
    }
  $str .= '>' . $title . "</h2>\n" . 
    '<a href="#" id="hide-button" class="align-right" onClick="HideShowElt(this, ' . "'$id'" . 
    ",'" . _("show") . "','" . _("hide") . "'" . ');">' . 
    _("hide") . '</a>' . "\n" . "</div><div id=\"$id\" class=\"center\"></div></div>\n";
  return $str;
}

?>
