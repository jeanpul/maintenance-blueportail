<?php

include_once("BluePHP/System/NetworkInterface.inc");

class Network
{
  private $netParams = array( "NETDEV" => null,
			      "IPADDRESS" => null,
			      "IPMASK" => null,
			      "IPGATEWAY" => null,
			      "IPDNS" => null,
			      "HWADDR" => null );

  private $ifaces = array();
  private $autos = array();
  private $dns = array();


  public function __construct()
  {
  }

  public function setNetParams($params)    
  {
    $this->netParams = $params;
  }

  public function getNetParams()
  {
    return $this->netParams;
  }

  public function checkDevice($dev)
  {
    $ret = null;
    system("grep -q $dev /proc/net/dev", $ret);
    return $ret == 0;
  }

  public function obtainMACAddress($interface = "eth0")
  {
    $mac = "";
    exec('/sbin/ifconfig ' . $interface, $output);
    $output = preg_grep('/HWaddr/', $output);
    if(count($output))
      {
	$mac = preg_replace('/.*HWaddr .*(..:..:..:..:..:..).*/', '$1', $output[0]);
	$mac = strtoupper($mac);
      }
    return $mac;
  }

  public function setValues()
  {
    $str = "auto lo " . $this->netParams["NETDEV"] . "\n";
    $str .= "iface lo inet loopback\n";
    $str .= "iface " . $this->netParams["NETDEV"] . " inet static\n";
    $str .= "address " . $this->netParams["IPADDRESS"] . "\n";
    $str .= "netmask " . $this->netParams["IPMASK"] . "\n";
    $str .= "gateway " . $this->netParams["IPGATEWAY"] . "\n";

    if($this->netParams["HWADDR"])
      {
	$str .= "hwaddress ether " . $this->netParams["HWADDR"] . "\n";
      }
    file_put_contents("/tmp/interfaces.tmp", $str);
    exec("sudo cp /tmp/interfaces.tmp /etc/network/interfaces");

    file_put_contents("/tmp/resolv.conf.tmp", "nameserver " . $this->netParams["IPDNS"] . "\n" . 
		      "nameserver 127.0.0.1\n");
    exec("sudo cp /tmp/resolv.conf.tmp /etc/resolv.conf\n" . 
	 "sudo /etc/init.d/networking stop ; sudo service networking start");
  }

  public function getValues()
  {
    if(is_null($this->netParams["NETDEV"]))
      {
	$this->netParams["NETDEV"] = "eth0";
      }
    $handle = fopen("/etc/network/interfaces", "r");
    if($handle)
      {
	$this->parse_interfaces($handle);
	if(isset($this->ifaces[$this->netParams["NETDEV"]]))
	  {
	    $dev = $this->ifaces[$this->netParams["NETDEV"]];
	    $this->netParams["IPADDRESS"] = $dev->getParameter("address");
	    $this->netParams["IPMASK"] = $dev->getParameter("netmask");
	    $this->netParams["IPGATEWAY"] = $dev->getParameter("gateway");
	    $this->netParams["HWADDR"] = $dev->getParameter("ether");
	  }
	else
	  {
	    throw Exception("Cannot find network informations for device " . $this->netParams["NETDEV"]);
	  }
	fclose($handle);
      }
    else
      {
	throw Exception("Cannot open network interface configuration file");
      }
    $handle = fopen("/etc/resolv.conf", "r");
    if($handle)
      {
	$this->parse_dns($handle);
	if(count($this->dns))
	  {
	    $this->netParams["IPDNS"] = $this->dns[0];
	  }
	else
	  {
	    throw Exception("Cannot find dns information");
	  }
	fclose($handle);
      }
    return $this->netParams;
  }

  private function isDeclLine($line)
  {
    return $line != "" and substr($line, 0, 1) != "#";
  }

  private function parse_dns($handle)
  {
    $this->dns = array();
    while($line = fgets($handle))
      {
	$line = trim($line);
	if($this->isDeclLine($line))
	  {
	    $lineElts = explode(" ", $line);
	    if($lineElts and count($lineElts) > 1)
	      {
		if($lineElts[0] == "nameserver")
		  {
		    $this->dns[] = $lineElts[1];
		  }
	      }
	  }
      }
  }

  private function parse_interfaces($handle)
  {
    $curIface = null;
    while($line = fgets($handle))
      {
	$line = trim($line);
	if($this->isDeclLine($line))
	  {
	    $lineElts = explode(" ", $line);
	    if($lineElts and count($lineElts) > 1)
	      {	
		if($lineElts[0] == "auto")
		  {
		    unset($lineElts[0]);
		    $this->autos = $lineElts;
		    if(!is_null($curIface))
		      {
			$this->ifaces[$curIface->getDev()] = $curIface;
			$curIface = null;
		      }
		  }
		else if($lineElts[0] == "iface")
		  {
		    if(!is_null($curIface))
		      {
			$this->ifaces[$curIface->getDev()] = $curIface;
		      }
		    $curIface = new NetworkInterface($lineElts[1]);
		  }
		else if($curIface)
		  {
		    $curIface->setParameter($lineElts[0], $lineElts[1]);
		  }
		else
		  {
		    throw Exception("Unknown identifier " . $lineElts[0]);
		  }
	      }
	    else
	      {
		throw Exception("Incorrect declaration");
	      }
	  }
      }
    if($curIface)
      {
	$this->ifaces[$curIface->getDev()] = $curIface;
      }
  }

  private function get_elt_value($name, $line)
  {
    $value = null;
    $lineElts = explode(" ", $line);
    if($lineElts and count($lineElts) > 0)
      {
	$value = trim( $lineElts[1] );
      }
    else
      {
	throw Exception("Incorrect $name decl");
      }
    return $value;
  }

};

?>