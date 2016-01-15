<?php

/**
 * \class Container
 * Base interface to describe a container
 * which is just something that have 
 * a add method to put an element into the container
 */
interface Container
{
  /**
   * Put an element into the container
   */
  public function add($elt);
}

?>