<?php

function sql_create_valuelist($data, $quoteStyle = "'")
{
  $sep = "";
  $str = "";
  foreach($data as $v)
    {
      if(!is_numeric($v))
	{
	  $v = $quoteStyle . $v . $quoteStyle;
	}
      $str .= $sep . $v;
      $sep = ",";
    }
  return $str;
}

?>