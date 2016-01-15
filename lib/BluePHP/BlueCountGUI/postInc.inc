<?php

/**
 * \file postInc.inc
 * Will be included at the end of every page.
 * $mainFrame variable must exist and it's
 * the contents of the main part of the page. Others parts
 * will be automatically generated.
 * Do not forget to include preInc.inc at
 * the begining of the page to get apropriate
 * includes
 */

// check admin access to display the Admin menu entry
if($session["access"] >= ADMINACCESS)
{
  $Menu[getBluePortailURL("adminUser.php")] = _("Admin");
}

$Menu[getBluePortailURL("help.php")] = _("Help");

$topLeftInfos = formMenu(array("_softname" => SOFTNAME,
			       "_revision" => trim(VERSION,'$')),
			 "Left");

$mainMenu = array("_user" => _("User") . " " . $session["login"],
		  //		  "_date" => array( "UpdateTime", strftime(_("%A %e %B %Y %Hh"))));
		  "_date" => array("NTPClock", "" ));
$mainMenu = array_merge($mainMenu, $Menu, array( getBluePortailURL("logout.php") => _("Logout") ));
//getBluePortailURL("/Dashboards/index.php") => _("DashBoard") ));

$topRightInfos = formMenu($mainMenu,
			  "Right");

echo '<div id="Logo"><img src="styles/' . $configSharedData["LOGO_RELATIVE_PATH"]["value"] . '" alt="Logo"/></div>' . "\n";
echo '<div id="TopMenu" class="Bars">' . '<div class="LeftPart">' . $topLeftInfos . '</div><div class="RightPart">' . $topRightInfos . '</div>' . "</div>\n";
echo $frame;
echo '<div id="BottomMenu" class="Bars">' . formMenu(array("" => COPYRIGHT), "BottomMenu") . "</div>\n";

$glang->close();

//$config->close();

$ntpParams = "login=" . $session["login"] . "&access=" . $session["access"];
if($clientBluePortail)
{
  $ntpParams .= "&clientBluePortail=" . $clientBluePortail;
}

echo "<script language=\"javascript\">updateNTPDate(\"NTPClock\", 60000, \"$ntpParams\");</script>\n";

?>

</div>
</body>
</html>
