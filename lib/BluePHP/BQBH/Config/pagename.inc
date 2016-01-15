<?php

// COMMON ERROR MESSAGE
$MissingParametersMessage = _("Internal error : Missing parameters.");

$BackPageTitle = array(
    '/cgi-bin/BlueSystem/index.cgi' => _("Back to main page"),
    'BQBHServerList.php' => _("Back to server list"),
    'BQBHServerMenu.php' => _("Back to server menu"),
    'BQBHSensor.php' => _("Back to sensor list"),
    'BQBHClientId.php' => _("Back to client id list"));

function returnToServerList(&$pageparams)
{
    $pageparams['backButtonUrl'] = "BQBHServerList.php";
    $pageparams['backButtonTitle'] = $BackPageTitle[$pageparams['backButtonTitle']];
}

function returnToServerMenu(&$pageparams)
{
    $pageparams['backButtonUrl'] = "BQBHServerMenu.php";
    $pageparams['backButtonTitle'] = $BackPageTitle[$pageparams['backButtonTitle']];
}

function returnToSensorList(&$pageparams)
{
    $pageparams['backButtonUrl'] = "BQBHSensor.php";
    $pageparams['backButtonTitle'] = $BackPageTitle[$pageparams['backButtonTitle']];
}

?>