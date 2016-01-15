<?php

/**
 * Common mathematic operations
 */


/**
 * Compute a percentage of the value a in b 
 * taking in account the 0 value and return
 * with 2 decimal precision.
 */
function computePercentage($a, $b)
{
  if($a == 0)
    { // avoid division by 0
      return 0;
    }
  return round($b / $a * 100., 2);
}


?>