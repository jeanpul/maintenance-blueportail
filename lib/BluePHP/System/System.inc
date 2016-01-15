<?php

class System
{
  public function reboot()
  {
    exec('sudo /sbin/telinit 6 > /dev/null 2> /dev/null');
  }

  public function getHardDisks()
  {
    $hds = array();
    exec("grep -e \"sd.$\" -e \"hd.$\" /proc/partitions | awk -F ' ' '{print $4}'", $hds);
    return $hds;
  }
}

?>