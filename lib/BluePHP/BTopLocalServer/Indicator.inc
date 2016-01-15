<?php

interface Indicator
{
  /**
   * Returns wheter or not the indicator value
   * stored in the database is computed by the process 
   * pushAndProcessTasks (normally located into /etc/BEV/scripts) 
   *
   * return false means that this indicator must return the value
   * data that will be stored into the database
   * @see Numbering_zonecounting
   */
  public function computedByPushAndProcess();

  /**
   * Re-compute the indicator value
   * and update the DB
   */
  public function computeInDB($params);

  /**
   * Returns an array of data from the DB
   */
  public function getValuesInDB($params);
}

?>