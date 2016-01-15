<?php

class Logs {

  function Logs()
  {
    openlog("BlueSMS Logs : ", LOG_PID, LOG_USER);
  }

  function process($api, $params)
  {
    return syslog(LOG_INFO, "BlueSMS send by Logs api id=" . $params["id"] . "&to=" . $params["dst"] . "&txt=" . $params["content"]);
  }

}

?>
