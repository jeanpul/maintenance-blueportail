<?php

include_once('BluePHP/BQBH/Process/ServerConfig.inc');
include_once('BluePHP/BQBH/Process/Config.inc');
include_once('BluePHP/BQBH/Config/pagename.inc');

$pageparams['backButtonTitle'] = $BackPageTitle[$pageparams['backButtonUrl']];

if(!testParams(array("server"), $params))
{
    $pageparams['backButtonUrl'] = "BQBHServerList.php";
    $pageparams['backButtonTitle'] = $BackPageTitle[$pageparams['backButtonUrl']];
    errorMessageAndExit($pageparams, $MissingParametersMessage);
}

$pageparams['backButtonFormComp'] .= Forms_inputHidden('server', $params['server']);

$server = new ServerConfig(BQBHSERVER_PATH, $params['server']);



?>