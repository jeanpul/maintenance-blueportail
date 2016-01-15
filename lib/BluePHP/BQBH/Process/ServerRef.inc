<?php

class ServerRef
{
    public $id;
    public $clientId;

    public function __construct($clientId, $id)
	{
	    $this->id = $id;
	    $this->clientId = $clientId;
	}

    public function getServerName()
	{
	    return "server_" . $this->id;
	}

    public function getId()
    {
      return $this->id;
    }

    public function getClientId()
    {
      return $this->clientId;
    }
}

?>
