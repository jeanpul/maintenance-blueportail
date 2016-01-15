<?php

function BSGTemplate_defaultBaseTextPageParams()
{
    return array_merge(array('baseTextPageContent' => ""),
		       defaultHtmlPageEndParams(),
		       defaultHtmlPageStartParams(),
		       defaultGetBackButtonParams());
}

function BSGTemplate_baseTextPage($params)
{
    $str = htmlPageStart($params);
    $str .= $params['baseTextPageContent'];
    $str .= getBackButton($params);
    $str .= htmlPageEnd($params);
    return $str;
}


function BSGTemplate_defaultStandardTextPageParams()
{
    return array_merge(array('standardTextPageContent' => ""),
		       defaultHtmlPageEndParams(),
		       defaultHtmlPageStartParams(),
		       defaultGetBackButtonParams());
}

function BSGTemplate_standardTextPage($params)
{
    $str = "<div id=\"formWide\">\n";
    $str .= $params['standardTextPageContent'];
    $str .= "</div>\n";
    $params['baseTextPageContent'] = $str;
    return BSGTemplate_baseTextPage($params);
}


function BSGTemplate_defaultErrorPageParams()
{
    return array_merge(array('errorPageMessage' => "ERROR"),
		       defaultHtmlPageEndParams(),
		       defaultHtmlPageStartParams(),
		       defaultGetBackButtonParams());
}

function BSGTemplate_errorPage($params)
{
  
  $params['baseTextPageContent'] = getErrorMessage($params['errorPageMessage']);
  return BSGTemplate_baseTextPage($params);
}

function BSGTemplate_defaultInfoPageParams()
{
    return array_merge(array('infoPageMessage' => "INFO"),
		       defaultHtmlPageEndParams(),
		       defaultHtmlPageStartParams(),
		       defaultGetBackButtonParams());
}

function BSGTemplate_infoPage($params)
{
  
  $params['baseTextPageContent'] = getInfoMessage($params['infoPageMessage']);
  return BSGTemplate_baseTextPage($params);
}

?>