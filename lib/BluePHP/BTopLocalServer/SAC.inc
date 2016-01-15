<?php

include_once("Config.inc");

class SAC 
{
  private $DIR = null;
  private $MAX_KEPT = 30;
  private $WGET_TIMEOUT = 10;
  private $WGET_RETRIES = 1;
  private $PAUSE_BETWEEN_SEND=1;

  public function __construct()
  {
    $this->DIR = BTOP_SERVER_DBPATH;
    $this->DIR = $this->DIR . "/SAC/";
  }

  public function send($day, $url)
  {
    $ret = 0;
    $wgetcmd = "wget --timeout=" . $this->WGET_TIMEOUT . " --tries=" . $this->WGET_RETRIES 
      . " \"$url\" -O /dev/null 2> /dev/null";
    system($wgetcmd, $ret);
    if($ret != 0)      
      {
	$this->save($day, $url);
      }
    return $ret;
  }

  public function purge()
  {
    foreach(glob($this->DIR . "/*.data") as $filename)
      {
	
      }
  }

  protected function create($filename)
  {
    if(!is_dir($this->DIR))
      {
	mkdir($this->DIR);
      }

    $findCmd = "find " . $this->DIR . " -name '*.data'";
    $nbExisting = system($findCmd . " | wc -l");
    if($nbExisting !== FALSE)
      {
	if($nbExisting > $this->MAX_KEPT)
	  {
	    $nbDiff = $this->MAX_KEPT - $nbExisting + 1;
	    system("for f in $($findCmd | sort | head -n $nbDiff) ; do rm -f \$f ; done");
	  }
      }
    touch($filename);
  }

  protected function save($day, $aline)
  {
    if($this->MAX_KEPT > 0)
      {
	if($aline != "")
	  {
	    $filename = $this->DIR . "/" . $day . ".data";
	    if(!file_exists($filename))
	      {
		$this->create($filename);
	      }
	    file_put_contents($filename, $aline . "\n", FILE_APPEND);
	  }
      }
  }  
}

?>