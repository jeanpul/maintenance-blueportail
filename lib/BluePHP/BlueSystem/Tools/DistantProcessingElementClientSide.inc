<?php

/**
 * \class DistantProcessingElementClientSide
 * \addtogroup BlueSystem
 * @{
 */
class DistantProcessingElementClientSide
{
  public $params;
  public $result;

  public function __construct($params)
  {
    $this->params = $params;
  }
  
  public function checkParams($keys)
  {
    return Utils_checkKeysInArray($keys, $this->params);
  }

  public function setResult($result)
  {
    $this->result = $result;
  }

  public function setResultPart($status, $output)
  {
    $this->result = array(
			  "status" => $status,
			  "output" => $output
			  );
    
  }

  public function displayAnswer()
  {
    $STATUS_REQUEST="";
    foreach($this->params as $n => $v)
      {
	$STATUS_REQUEST .= "[$n = $v]";
      }

    echo "STATUS_REQUEST=" . $STATUS_REQUEST . "\n";

    echo "STATUS=" . $this->result["status"] . "\n";    
    foreach($this->result["output"] as $outputline)
      {
	echo "STATUS_MESSAGE=\"" . $outputline . "\"\n";
      }
  }
};

/** @} */

?>