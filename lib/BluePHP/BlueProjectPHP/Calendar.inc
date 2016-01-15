<?php

function getDaysInMonth($y, $m) 
{
  $date = getdate(mktime(0,0,0,$m+1,0,$y));
  return $date["mday"];
}

?>