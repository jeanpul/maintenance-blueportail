<?php

include_once('BlueSystemGuiPHP/CommonOps.inc');

class ConfigManipGui
{
  protected $actions;
  protected $commonkeys;

  public function ConfigManipGui()
  {
    $this->commonkeys = array();
    $this->actions = array();
  }

  public function validAction($action)
  {
    if(!isset($this->actions[$action]))
      {
	die('ConfigManipGui::link : unknown action ['.$action.']');
	return false;
      }
    return true;
  }

  protected function gen_link($url, $params)
  {
    $first = true;
    $str = $url . '?';
    foreach($params as $k => $v)
      {
	if($first)
	  {
	    $str .= $k . '=' . $v;
	    $first = false;
	  }
	else
	  {
	    $str .= '&amp;' . $k . '=' . $v;
	  }
      }
    return $str;
  } 
  
  public function generatePage($params, &$pageparams)
  {
    if(testParams($this->commonkeys, $params))
      {
	$action = $params['action'];
	if(isset($this->actions[$action]))
	  {
	    $this->returnPage(false, $params, $pageparams);
	    
	    if(testParams($this->actions[$action]['keys'], $params))
	      {
		$pageparams['pageStartPageTitle'] = $this->actions[$action]['title'];
		$method = 'do_' . $action;
		$pageparams['standardTextPageContent'] .= $this->$method($params);
		return;
	      } 
	  }
      }
    $this->internalError($pageparams);
  }
  
  protected function internalError(&$pageparams)
  {
    global $MissingParametersMessage;
    $this->returnPage(true, null, $pageparams);    
    errorMessageAndExit($pageparams, $MissingParametersMessage);
  }
  
  protected function actionDone($str)
  {
    return '<p>'.$str.'</p>';
  }

  protected function removalConfirmationPage($msg, $params)
  {
    $str = '<p>' . $msg . "</p>\n";
    $elts = Forms_multipleInputHidden($params);
    $elts .= Forms_submitButton(_('Apply'));
    $str .= '<p>'.Forms_formGet('', $elts).'</p>';
    return $str;
  }

  protected function returnPage($error, $params, &$pageparams)
  {    
  }
  
};

?>