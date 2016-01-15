<?php

include_once("BluePHP/Utils/js_lang.inc");
include_once("BluePHP/GUI/MenuBarEntry.inc");
include_once("BluePHP/JQUERY/JQueryApplication.inc");
include_once("BluePHP/BlueSystem/BlueSystem.inc");

function BS_callClassHTML($options)
{
  return js_funcall( array( "function" => "BlueSystem.callClassHTML",
			    "options" => $options ) );
}

function BS_eventRender($remoteClass, $params = array())
{
  return js_funcall(array( "function" => "BlueSystem.callClassHTML",
			   "options" => array( "class" => $remoteClass,
					       "render" => "BSApplication",
					       "mode" => "append",
					       "params" => $params )));
}

function BS_eventRenderWithForm($remoteClass, $id, $formId, $function = "render")
{
  return js_funcall(array( "function" => "BlueSystem.ModuleFormApply",
			   "options" => array( "class" => $remoteClass,
					       "render" => "BSApplication",
					       "mode" => "append",
					       "function" => $function,
					       "formId" => $formId,
					       "id" => $id )));
}

function BS_eventRenderWithParams($remoteClass, $id, $params, $function = "render")
{
  return js_funcall(array( "function" => "BlueSystem.ModuleParamsApply",
			   "options" => array( "class" => $remoteClass,
					       "render" => "BSApplication",
					       "mode" => "append",
					       "function" => $function,
					       "params" => $params,
					       "id" => $id )));
}

/**
 * This is the base class that manages
 * the root part of the GUI. This contains
 * shared components (headers, footers, ...)
 * and common dynamic contents such as ref, lang
 * logo...
 */
class BSApplication extends JQueryApplication
{
  private $bs = null;
  private $params = null; // retrieve parameters from the cli or the url options
  private $requiredParams = null;
  private $navigation = array();
  private $systems = array();

  public function __construct($win = null, $requiredParams = null, $path = "BluePHP/BlueSystem/GUI/templates/")
  {
    parent::__construct($win, $path);
    $this->extract_params();
    $this->addRenderStyle("/BlueSystem/styles/jquery-ui-theme.css");
    $this->addRenderStyle("/BlueSystem/styles/StyleBlueEyeVideo.css");
    $this->addRenderStyle("/BlueSystem/styles/dataTables.css");
    $this->addLib("lazyload-min", array("version" => null, "path" => "/JQUERY/"));
    $this->addLib("BlueSystem", array("version" => null, "path" => "/BlueSystem/js/"));
    $this->bs = new BlueSystem();
    $this->requiredParams = $requiredParams;
  }

  private function extract_params()
  {
    global $argv;
    $this->params = array();
    if(PHP_SAPI == 'cli')
      {
	for($i = 2; $i < count($argv); $i += 2)
	  {
	    $this->params[$argv[$i - 1]] = $argv[$i];
	  }
      }
    else
      {
	$this->params = $_REQUEST;
      }
  }

  public function initDialogModule($params)
  {
    $params = array_merge(array("id" => "Default",
				"module" => "BlueSystem"), $params);
    $this->pushReadyJS("BlueSystem.CreateDialog({'id':'" . $params["id"] . "'});");
    $this->pushReadyJS("BlueSystem.ModuleInit(" . json_encode($params) . ");");
  }

  public function eventRender($remoteClass, $params = "")
  {
    return BS_eventRender($remoteClass, $params);
  }

  public function getReference()
  {
    return $this->bs->getReference();
  }

  public function getClientId()
  {
    return $this->bs->getClientId();
  }

  public function getCopyrightYear()
  {
    return $this->bs->getCopyrightYear();
  }

  public function getBlueSystem()
  {
    return $this->bs;
  }

  public function getParams()
  {
    return $this->params;
  }

  public function setNavigationElts($elts)
  {
    $this->navigation = $elts;
  }

  public function setSystemElts($elts)
  {
    $this->systems = $elts;
  }

  public function getNavigationElts()
  {
    return $this->navigation;
  }

  public function getSystemElts()
  {
    return $this->systems;
  }

  public function setRequiredParams($params)
  {
    $this->requiredParams = $params;
  }
  
  public function getMissingParams()
  {
    $res = null;
    if($this->requiredParams)
      {
	if(is_null($this->params) or !count($this->params))
	  {
	    $res = $this->requiredParams;
	  }
	else
	  {
	    $res = array();
	    foreach($this->requiredParams as $rp)
	      {
		if(!array_key_exists($rp, $this->params))
		  {
		    array_push($res, $rp);
		  }
	      }
	    if(!count($res))
	      {
		$res = null;
	      }
	  }
      }
    return $res;
  }
}

?>