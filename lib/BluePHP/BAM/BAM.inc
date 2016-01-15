<?php

include_once("BluePHP/Utils/DBConnect.inc");

// class that handle the 
// BlueEyeVideo Advanced Message system
class BAM
{
  // global parameters 
  // threshold mean differents things depending on the type
  // for info that means the duration (mn) where the message stay
  // stored in the db
  // for warning/error means the number of message per sender that will
  // be stored in the db
  // future version could extend this
  private $globalParams = array( "info" => array( "threshold" => 5 ),
				 "warning" => array( "threshold" => 1 ),
				 "error" => array( "threshold" => 1 ) ); 
  private $con = false;
  protected $conParams = false;
  
  function __construct($conParams)
  {
    $this->conParams = $conParams;
  }

  public function open()
  {
    $this->con = new DBConnect($this->conParams["type"],
			       $this->conParams["host"],
			       $this->conParams["name"],
			       $this->conParams["user"],
			       $this->conParams["passwd"]);
    $this->con->connectToDB();
    return $this->con;
  }

  public function close()
  {
    if($con)
      {
	$this->con->close();
      }
  }

  protected function msg_print_generic($module, $type, $message)
  {
    if(!$this->con)
      {
	$this->open();
      }

    if($type != "info")
      {
	// this is a message queue where threshold represent the
	// message position in the queue so msg outise the limit size
	// will be removed during the update session
	$this->update_other($type, $module);
      }
    $date = strftime("%Y%m%d%H%M%S");
    $msgId = md5($date . $module . $type . $message);
    $query = "insert into Msgs (\"msgId\", \"dateSend\", \"module\", \"type\", \"message\", \"threshold\") " . 
      "values(\"$msgId\", \"$date\", \"$module\", \"$type\", \"$message\", 0)";
    $result = $this->con->query($query);
    $result->freeResult();
  }

  public function Msg_info($module, $message)
  {
    $this->msg_print_generic($module, "info", $message);
  }

  public function Msg_warning($module, $message)
  {
    $this->msg_print_generic($module, "warning", $message);
  }

  public function Msg_error($module, $message)
  {
    $this->msg_print_generic($module, "error", $message);
  }

  public function update()
  {
    if(!$this->con)
      {
	$this->open();
      }

    // remove messages above threshold
    // FAB: trigger is currently not used to be compatible with all sql database 
    // update info threshold
    $this->update_info();
    $this->delete_msgs();
    $this->delete_clients();
  }

  public function Msg_get($clientId, $module)
  {
    if(!$this->con)
      {
	$this->open();
      }

    $query = "select * from Msgs where not " . $this->alive_msgs_expr() . 
      " and not exists (select * from MsgClient where clientId=\"$clientId\" and " . 
      "MsgClient.msgId=Msgs.msgId) and module like \"$module\"";
    $result = $this->con->query($query);
    $msgs = array();
    while($obj = $result->nextAssoc())
      {
	$msgs[$obj["msgId"]] = array_merge($obj, array("clientId" => $clientId));	
      }
    $result->freeResult();

    $this->delete_msgs();

    $query = "insert or replace into MsgClient select msgId, \"$clientId\" from Msgs where module like \"$module\"";
    $result = $this->con->query($query);
    $result->freeResult(); 

    return $msgs;
  }
  
  private function alive_msgs_expr()
  {
    return "((type=\"info\" and threshold > " . $this->globalParams["info"]["threshold"] . ") or " . 
      "(type=\"warning\" and threshold > " . $this->globalParams["warning"]["threshold"] . ") or " .
      "(type=\"error\" and threshold > " . $this->globalParams["error"]["threshold"] . "))";
  }

  private function delete_msgs()
  {
    $query = "delete from Msgs where " . $this->alive_msgs_expr();
    $result = $this->con->query($query);
    $result->freeResult(); 
  }

  private function delete_clients()
  {
    $query = "delete from MsgClient where not exists (select * from Msgs where Msgs.msgId=MsgClient.msgId)";
    $result = $this->con->query($query);
    $result->freeResult(); 
  }

  private function update_other($type, $module)
  {
    $query = "update Msgs set threshold=threshold+1 where type=\"$type\" and module=\"$module\"";
    $result = $this->con->query($query);
    $result->freeResult(); 
  }

  private function update_info()
  {
    $query = "update Msgs set threshold=threshold+1 where type=\"info\"";
    $result = $this->con->query($query);
    $result->freeResult(); 
  }
}

?>
