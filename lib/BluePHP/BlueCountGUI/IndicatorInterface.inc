<?php

interface IndicatorInterface
{
  public function getId();

  public function getType();

  public function getInputParameters();

  public function getURLInputParameters();

  public function getParamsContents();

  public function getDataContents();
}

?>