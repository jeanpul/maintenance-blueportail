<?php

include_once("BluePHP/BTopLocalServer/BCMClient.inc");
include_once("BluePHP/BTopLocalServer/Config.inc");

class BCMXMLFileClient extends BCMClient
{
  private $parser = false;

  public function __construct()
  {    
    $this->parser = xml_parser_create();
    xml_parser_set_option($this->parser, XML_OPTION_CASE_FOLDING,0);
  }

  protected function genericProcessing($f, $tag, $extraParams = null)
  {
    $res = array();

    $xml = file_get_contents($this->getFilename("$f", $extraParams));
    $ret = xml_parse_into_struct($this->parser, $xml, $vals, $tags);

    print_r($tags);

//    if($ret == 0)
//      {
//	var_dump(xml_error_string(xml_get_error_code($this->parser)));
//      }
//
//    foreach($tags as $k => $v)
//      {
//	if($k == "$tag")
//	  {
//	    $elt = array();
//	    $curId = false;
//	    for($i = 0; $i < count($v) - 1; ++$i)
//	      {
//		$offset = $v[$i] + 1;
//		$len = $v[$i + 1] - $offset;
//		if($len > 0)
//		  {
//		    $attr = $this->parseSlice(array_slice($vals, $offset, $len));
//		    if(isset($attr["id"]))
//		      {
//			$curId = $attr["id"];	
//			$res[$curId] = array();
//		      }
//		    $res[$curId] = array_merge( $res[$curId], $attr );
//		  }
//	      }
//	  }
//      }
    return $res;
  }

  private function parseSlice($slice)
  {
    $res = null;
    for($i = 0; $i < count($slice); ++$i)
      {
	$res[$slice[$i]["tag"]] = $slice[$i]["value"];
      }
    return $res;
  }

  public function getLastMinuteValues()
  {
    return $this->genericProcessing("getLastMinuteValues",
				    "BCMServerData");
  }

  protected function getFilename($f, $extraParams = null)
  {
    return DATADBPATH . "/OpenData/QMS/getLastMinuteValues";
  }
}

?>
