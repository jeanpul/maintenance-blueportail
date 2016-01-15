<?php

/**
 * \class Component
 * This is the base interface of all renderable
 * element. A renderable element is a class that
 * provides the method render in order to display
 * something
 * \todo should change the name to Renderable or something like that
 */
interface Component
{
  /**
   * Called in order to display something
   */
  public function render();
}

?>