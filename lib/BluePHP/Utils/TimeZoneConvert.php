<?php

// convert a date in the format YYYYMMDDHHMMSS 
// from a specific TimeZone to another TimeZone

if($argc != 4)
{
  ?>

Convert a date in the format YYYYMMDDHHMMSS from a specific TimeZone to another TimeZone

  Usage :
  <?php echo $argv[0]; ?> YYYYMMDDHHMMSS tzSource tzDst
 
<?php
}
else
{
  $timestamp = $argv[1];
  $tzSrc = $argv[2];
  $tzDst = $argv[3];

  $dateTime = new DateTime( substr($timestamp, 0, 4) . "-" . 
			    substr($timestamp, 4, 2) . "-" . 
			    substr($timestamp, 6, 2) . " " .
			    substr($timestamp, 8, 2) . ":" .
			    substr($timestamp, 10, 2) . ":" .
			    substr($timestamp, 12, 2), new DateTimeZone($tzSrc) );
  $dateTime->setTimeZone(new DateTimeZone($tzDst));
  echo $dateTime->format("YmdHis") . "\n";
}
?>
