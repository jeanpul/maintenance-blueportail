<?php

function generateHREF($pageName, $refName, $params, $newWindow = false)
{
  $str = "<a href='$pageName";
  $sep = "?";
  foreach($params as $k => $v)
    {
      $str .= $sep . $k . "=" . $v;
      if($sep == "?")
	{
	  $sep = "&";
	}
    }
  $str .= "'";
  if($newWindow)
    {
      $str .= " target='_blank'";
    }
  $str .= ">$refName</a>";
  return $str;
}

function generateROW($rowValues, $tag = "td")
{
  $str = "<tr>";
  foreach($rowValues as $k => $v)
    {
      $str .= "<$tag>$v</$tag>";
    }
  $str .= "</tr>";
  return $str;
}

?>