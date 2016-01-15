<?php

class BSLicenseFile
{
  public $filename;
  public $prop;
  
  public function __construct($file)
  {
    $this->filename = $file;
    unset($this->prop);
  }

  public function load($mac)
  {
    $res = 0;
    $this->prop = array('READ_STATUS' => 1,
			'TYPE' => "",
			'START' => "",
			'END' => "",
			'NB' => 0);
    
    if(!file_exists($this->filename))
      {
	$this->prop['READ_STATUS'] = 1;
	$res = 1;
      }
    else
      {
	$cmdline = '/usr/bin/readLicense ' . $this->filename . ' ' . $mac;
	exec($cmdline, $output, $res);
	if($res == 255 || $res == 1) 
	  {
	    $this->prop['READ_STATUS'] = 2;	    
	  }
	else
	  {
	    $this->prop['READ_STATUS'] = 0;
	    $eltstr = preg_replace('/.*([a-zA-Z0-9]+) ([a-zA-Z0-9]+) ([a-zA-Z0-9]+) ([a-zA-Z0-9]+).*/', 
				   '$elt=array("$1", "$2", "$3", "$4");', $output[0]);
	    eval($eltstr);
	    
	    $this->prop['TYPE'] = $elt[0];
	    $this->prop['START'] = $elt[1];
	    $this->prop['END'] = $elt[2];
	    $this->prop['NB'] = $elt[3];
	  }
      }
    return $res;
  }

  public function getNb()
  {
    return $this->prop["NB"];
  }
  
};

?>