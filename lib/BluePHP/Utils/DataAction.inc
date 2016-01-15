<?php

/**
 * \class DataAction
 * Base class "interface" for every class that manage some data
 * \addtogroup Utils
 * @{
 */
interface DataAction
{
  /**
   * Returns true if the action is to render the list
   * of elements
   */
  public function isRenderList();

  /**
   * Returns true if the action is to render the edition form
   */
  public function isRenderEdit();

  /**
   * Render the table 
   * that display the list of elements
   */
  public function renderList();

  /**
   * Render the form to
   * create/edit an element
   * \param $params element data
   */
  public function renderEdit($params);
  
}
/** @} */

?>