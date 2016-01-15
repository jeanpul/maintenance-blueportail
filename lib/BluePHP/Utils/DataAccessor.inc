<?php

/**
 * \class DataAccessor
 * Base class "interface" for every class that manage some data
 * \addtogroup Utils
 * @{
 */
interface DataAccessor
{
  /**
   * Returns an array of all the data
   */
  public function getEntries();

  /**
   * Return a specific entry
   * \param $params information used to retrieve the specified data
   */
  public function getEntry($params);

  /**
   * insert new data
   * \param $params data values
   */ 
  public function newEntry($params);

  /**
   * update existing data
   * \param $params new values
   */
  public function setEntry($params);

  /**
   * remove data
   * \param $params data idx to be removed
   */
  public function delEntry($params);  
}
/** @} */

?>