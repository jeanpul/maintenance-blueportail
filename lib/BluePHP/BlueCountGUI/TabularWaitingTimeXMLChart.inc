<?php

include_once("TabularFlowXMLChart.inc");

class TabularWaitingTimeXMLChart extends TabularFlowXMLChart
{
  // directly send XML data to chartmono.php
  function getData($params)
    {
      $datafile = "tmp/datagraph" . $params["id"] . $params["key"];
      $method = GRAPHIC_KIND . '_chartForWaitingTime';
      return $method($this, $datafile);
    }
}

?>