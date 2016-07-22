<?php

/**
 * Operations on date such as get the start and end of the week.
 * Get the start and end of this month...
 */

/**
 * Add time to an Unix time format with the values
 * specified in an array for each time step 
 * then returns the corresponding unix timestamp
 */
function addTime($timestamp, $timestep)
{
  return mktime((int) date('H', $timestamp) + $timestep['hour'], // hour
		(int) date('i', $timestamp) + $timestep['minute'], // minute
		(int) date('s', $timestamp) + $timestep['second'], // second
		(int) date('m', $timestamp) + $timestep['month'],  // month
		(int) date('d', $timestamp) + $timestep['day'], // day
		(int) date('Y', $timestamp) + $timestep['year']); // year
}

/**
 * Sub time to an Unix time format with the values
 * specified in an array for each time step 
 * then returns the corresponding unix timestamp
 */
function subTime($timestamp, $timestep)
{
  return mktime((int) date('H', $timestamp) - $timestep['hour'], // hour
		(int) date('i', $timestamp) - $timestep['minute'], // minute
		(int) date('s', $timestamp) - $timestep['second'], // second
		(int) date('m', $timestamp) - $timestep['month'],  // month
		(int) date('d', $timestamp) - $timestep['day'], // day
		(int) date('Y', $timestamp) - $timestep['year']); // year
}

/**
 * Returns an array composed of two entry
 * start end end that represents two dates into the
 * YmdHi format that is the start end the end date of
 * the day relativly of the timestamp specified. The
 * timestamp is the Unix time given by time or mktime (see PHP doc)
 * N.B : in this case start equal end
 */
function getStartEndDayDate($timestamp)
{
  $result = array( "start" => 0, "end" => 0 );

  $result["start"] = date('Ymd', $timestamp) . date('H', strtotime(TIMESTART)) . "00";
  $result["end"] = date('Ymd', $timestamp) . date('H', strtotime(TIMEEND)) . "00";

  return $result;
}

/**
 * Returns an array composed of two entry
 * start end end that represents two dates into the
 * YmdH format that is the start end the end date of
 * the week relativly of the timestamp specified. The
 * timestamp is the Unix time given by time or mktime (see PHP doc)
 */
function getStartEndWeekDate($timestamp)
{
  $result = array( "start" => 0, "end" => 0 );
  
  $result["start"] = date('YmdHi', mktime(date('H', strtotime(TIMESTART)), 
					  0, 0, 
					  date("m", $timestamp), 
					  date("d", $timestamp) - date("w", $timestamp), 
					  date("Y", $timestamp)));
  $result["end"] = date('YmdHi', mktime(date('H', strtotime(TIMEEND)), 
					0, 0, 
					date("m", $timestamp), 
					date("d", $timestamp) + 6 - date("w", $timestamp), 
					date("Y", $timestamp)));  
  return $result;
}


function getStartEndWeekDate2($timestamp)
{
  $nday = strftime("%u", $timestamp);
  return array( "start" => strftime("%Y%m%d%H%M%S", addTime($timestamp, getStepDay(1 - $nday))),
		"end" => strftime("%Y%m%d%H%M%S", addTime($timestamp, getStepDay(5 - $nday))) );
}


/**
 * Returns an array composed of two entry
 * start end end that represents two dates into the
 * YmdH format that is the start end the end date of
 * the month relativly of the timestamp specified. The
 * timestamp is the Unix time given by time or mktime (see PHP doc)
 */
function getStartEndMonthDate($timestamp)
{
  $result = array( "start" => 0, "end" => 0 );

  $result["start"] = date('YmdHi', mktime(date('H', strtotime(TIMESTART)), 
					  0, 0, 
					  date("m", $timestamp), 
					  1, 
					  date("Y", $timestamp)));
  $result["end"] = date('YmdHi', mktime(date('H', strtotime(TIMEEND)), 
					0, 0, 
					date("m", $timestamp), 
					date("t", $timestamp), 
					date("Y", $timestamp)));
  return $result;
}

/**
 * Returns an array composed of two entry
 * start end end that represents two dates into the
 * YmdHi format that is the start end the end date of
 * the time relatively of the timestamp specified and using
 * the crontab problem deviation. The
 * timestamp is the Unix time given by time or mktime (see PHP doc)
 */
function getStartEndCrontabDate($timestampStart, $timestampEnd)
{
  global $CRONSTEP;

  $result = array( "start" => 0, "end" => 0 );

  $result["start"] = date('YmdHi', addTime($timestampStart, $CRONSTEP));
  $result["end"] = date('YmdHi', addTime($timestampEnd, $CRONSTEP));

  return $result;
}

/**
 * Returns the unix timestamp from a string in 
 * YYYYMMDDhhmmss format.
 */
function mktimeFromString($timestamp)
{
  // construct the Unix timestamp format from the string timestamp
  // see php doc
  return mktime(substr($timestamp, -6, 2), // hour
		substr($timestamp, -4, 2), // minute
		substr($timestamp, -2, 2), // second
		substr($timestamp, 4, 2),  // month
		substr($timestamp, 6, 2),  // day
		substr($timestamp, 0, 4)); // year
}

/**
 * Remove time from a timestamp String in the
 * YYYYMMDDhhmmss format, then return the date
 * in the same format
 */
function addTimeFromString($timestamp, $timestep)
{
  return strftime("%Y%m%d%H%M%S", addTime(mktimeFromString($timestamp), array_merge( array("hour" => 0, 
											   "minute" => 0,
											   "second" => 0,
											   "month" => 0,
											   "day" => 0,
											   "year" => 0),
										     $timestep)));
}

/**
 * convert format YYYY-MM-DD HH:MM:SS
 * to YYYYMMDDhhmmss
 */
function convertCommonTimeString($timestamp)
{
  return str_replace(array("-", ":", " "), "", $timestamp);
}

/**
 * convert format YYYYMMDDhhmmss to
 * YYYY-MM-DD HH:MM:SS
 */
function convertToDBTimeFormat($timestamp)
{
  return substr($timestamp, 0, 4) . "-" . substr($timestamp, 4, 2) . "-" . 
    substr($timestamp, 6,2) . " " . substr($timestamp, 8, 2) . ":" . 
    substr($timestamp, 10, 2) . ":" . substr($timestamp, 12, 2);
}

/**
 * Apply a new time zone to
 * a date in the format YYYY-MM-DD HH:MM:SS
 */
function changeTimeFormatTimeZone($timestamp, $tz)
{
  $dateTime = new DateTime( $timestamp );
  $dateTime->setTimeZone(new DateTimeZone($tz));
  return $dateTime->format('Y-m-d H:i:s');
}

/**
 * Returns the difference between two timestamp
 * in seconds. The timestamp should be in the string
 * YYYYMMDDhhmmss format.
 * The result is an absolte value so don't be worried
 * of the arg order.
 */
function getDateSub($timea, $timeb)
{
  return abs(mktimeFromString($timea) - mktimeFromString($timeb));
}

/**
 * Returns the difference between two unix time
 * arguments are on the form YYYYMMDDhhmmss
 */
function getTimeSubFromString($timea, $timeb)
{
  return mktimeFromString($timea) - mktimeFromString($timeb);
}

/**
 * Returns an array of time slices
 * between TIMESTART and TIMEEND for each
 * TIMESTEP time long. 
 * Time slices values are unix timestamp 
 */ 
function getTimeSlices($timestamp = FALSE)
{
  global $TIMESTEP;

  if($timestamp)
    {
      $tstart = strtotime(TIMESTART, $timestamp);
      $tend = strtotime(TIMEEND, $timestamp);  
    }
  else
    {
      $tstart = strtotime(TIMESTART);
      $tend = strtotime(TIMEEND);  
    }
  
  $timeSlices = array();
  for($i = $tstart ; $i < $tend; )
    {
      $start = $i;
      $end = addTime($i, $TIMESTEP);
      $timeSlices[] = array( "start" => $start, "end" => $end );
      $i = $end;
    }
  return $timeSlices;
}

function getStepDay($nday)
{
  return array( "second" => 0, 
		"minute" => 0, 
		"hour" => 0, 
		"day" => $nday,
		"month" => 0,
		"year" => 0 );
}

function getTimeSlicesAsStrings($timestart, $timeend, $timestep)
{
  $res = array();
  while(getTimeSubFromString($timeend, $timestart) > 0)
    {
      $res[] = $timestart;
      $timestart = strftime("%Y%m%d%H%M%S", addTime(mktimeFromString($timestart), $timestep));
    }
  return $res;
}

/**
 * Returns TRUE if the specified string representation of the given
 * date is a JJ/MM/AAAA format and within 30 days from today
 */
function isGoodDate($date)
{
  list($day, $month, $year) = split('[/.-]', $date); 

  echo $day . "/" . $month . "/" . $year . "<br>";

  if(!checkdate($day, $month, $year))
    {
      return FALSE;
    }
  return mktime(0, 0, 0, $month, $day, $year) >
    mktime(0, 0, 0, date("m"), date("d") - MAX_DAY_HISTORY, date("Y"));
}

/**
 * Returns two arrays containing the MAX_DAY_HISTORY
 * dates with a time unix and string on the format specified by format parameter
 */
function getAcceptedDates($format)
{
  $unixDays = array();
  $unixDays[0] = time();
  
  $stringDays = array();
  $stringDays[0] = date($format);
  
  $lastDay = 1;
  for($i = 1; $i < MAX_DAY_HISTORY; $i++, $lastDay++)
    {
      $unixDays[$i] = mktime(0, 0, 0, date("m"), date("d") - $lastDay, date("Y"));
      if(date("l", $unixDays[$i]) == "Sunday")
	{
	  $lastDay++;
	  $unixDays[$i] = mktime(0, 0, 0, date("m"), date("d") - $lastDay, date("Y"));
	}
      $stringDays[$i] = date($format, $unixDays[$i]);
    }
  return array($unixDays, $stringDays);
}

?>
