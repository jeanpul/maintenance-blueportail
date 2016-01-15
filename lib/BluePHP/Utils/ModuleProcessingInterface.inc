<?php

interface ModuleProcessingInterface
{
  /**
   * Returns an array containing all entries
   * stored by the module
   */
  public function getEntries();

  /**
   * Returns an subset of data regarding
   * the specified parameters
   */
  public function getEntry($params);    

  /**
   * Insert new data into the module
   */
  public function newEntry($params);

  /**
   * Update existing data
   */
  public function setEntry($params);

  /**
   * Delete data from the module
   */
  public function delEntry($params);

  /**
   * Returns the URL string parameters used to retrieve a particular entry
   * TODO : replace this with getKeyParams where only key parameter
   * will be returned as an associative array so the URL will be
   * generated into the ModuleInterface class
   */ 
  public function getURLEntry($params);
}

?>