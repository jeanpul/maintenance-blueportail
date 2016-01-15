<?php

/**
 * Simply returns a dashed link
 * \deprecated to be replaced by a more general anchor tag
 */
function getMenuBarLink($id, $menuParams, $title)
{
  $hrefDashed = '#' . http_build_query($menuParams);
  return "<a id=\"$id\" href=\"$hrefDashed\">$title</a>";
}

?>