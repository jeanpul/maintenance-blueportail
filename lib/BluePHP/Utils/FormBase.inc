<?php

/**
 * \file FormBase.inc
 * \brief Handle HTML formular
 */

/**
 * \class FormBase
 * \brief Handle HTML formular
 */
class FormBase
{
  /**
   * Method used to send the parameters
   */
  var $method = "get";

  /**
   * link to call on submit
   */
  var $action = "";

  /**
   * \brief Create the structure
   * use the modifier to change 
   * the parameters
   */
  function FormBase($action = "", $method = "get")
    {
    }

  /**
   * \brief Set the method to use
   * \param $method the method to use either 'get' or 'put'
   * 
   * 'get' is the default method used
   */
  function setMethod($method)
    {
      if($method != 'get' and
	 $method != 'put')
	{
	  exit("FormBase::setMethod error: " . "unknown method $method, must be put or get");
	}
      $this->method = $method;
    }
  
  /**
   * \brief Set the action to call on submit
   * \param $action the link to call
   */
  function setAction($action)
    {
      $this->action = $action;
    }

  /**
   * \brief Returns the opening part of the form
   *
   * This returns <form action="$action" method="$get">
   */
  function begin()
    {
      return '<form action="' . $this->action . '" method="' . $this->method . '">' . "\n";
    }

  /**
   * \brief Returns the close part of the form
   *
   * This simply returns </form>
   */
  function end()
    {
      return "</form>\n";
    }
}

?>
