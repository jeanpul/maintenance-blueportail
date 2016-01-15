<?php

include_once("BluePHP/Utils/ModuleProcessing.inc");

class Users extends ModuleProcessing
{
  function Users()
  {
    parent::setParams( array( "name" => "Users",
			      "keyname" => "id",
			      "sort" => "name" ));
  }

  function getTel($params)
  {
    return parent::Select("tel from Users where id=" . $params["id"]);
  }

  function getEmail($params)
  {
    return parent::Select("email from Users where id=" . $params["id"]);
  }
}

?>

