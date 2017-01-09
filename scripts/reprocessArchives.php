<?php

function run_sql_exec($sql, $db) {

    exec("echo \"$sql\" | sqlite3 $db", $output);
    return $output;
}

function process_task($taskCmd, $root) {
    return passthru("$taskCmd $root processTasks");
}

$params = array(
    "hstart" => "08",
    "hend" => "19",
    "root" => "/home/DATA/BTopLocalServer/",
    "archives" => "/home/DATA/BTopLocalServer/Archives/",
    "bcltask" => "/usr/bin/BCLTask"    
);

$pFile = $params["root"] . "processed.db";

$fcps = run_sql_exec("select id from FlowCountingProcessing;", $pFile);
$zcps = run_sql_exec("select id from ZoneCountingProcessing;", $pFile);

foreach(glob($params["archives"] . "counters_*.db") as $archive) {

    $counters = $params["root"] . "counters.db";
    
    // copy to main counters
    copy($archive, $counters);

    $taskFile = $params["root"] . "task.db";

    // select fresh counters
    $fresh = run_sql_exec("select distinct strftime('%Y%m%d%H%M00', time) from counting where state=0 and strftime('%H', time) >= '" .
    $params["hstart"] . "' and strftime('%H', time) <= '" .
    $params["hend"] . "' order by time asc;", $counters);

    foreach($fresh as $start) {

        run_sql_exec("delete from tasks;", $taskFile);
        
        $sql = "begin transaction;";
        
        foreach($fcps as $fcp) {
            $sql .= "insert into tasks (str) values('FlowCounting MINUTE $fcp $start');";
        }

        foreach($zcps as $zcp) {
            $sql .= "insert into tasks (str) values('ZoneCounting MINUTE $zcp $start');";
            process_task($params["bcltask"], $params["root"]);
        }

        $sql .= "commit;";
        
        run_sql_exec($sql, $taskFile);
        process_task($params["bcltask"], $params["root"]);
    }
    
}

?>
