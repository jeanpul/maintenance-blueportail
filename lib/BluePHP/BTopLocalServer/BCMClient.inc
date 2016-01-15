<?php

abstract class BCMClient
{  
  public function getProcessingValues()
  {
    return $this->getNumberingProcessing();
  }

  public function getNumberingProcessing($extraParams = null)
  {
    return $this->genericProcessing("getNumberingProcessing", 
				    "NumberingProcessingElt",
				    $extraParams);
  }

  public function getLastMinuteNumberingValues()
  {
    return $this->genericProcessing("getLastMinuteValues",
				    "NumberingValuesElt");
  }

  public function getLastMinuteWaitingTimeValues()
  {
    return $this->genericProcessing("getLastMinuteValues",
				    "WaitingTimeValuesElt");
  }

  public function getLastDayNumberingValues()
  {
    return $this->genericProcessing("getLastDayValues",
				    "NumberingValuesElt");
  }

  abstract protected function genericProcessing($f, $tag, $extraParams = null);
}

?>