#!/bin/sh

. /etc/BEV/scripts/BlueSystem/Logs.inc
. /etc/BEV/scripts/BTopLocalServer/BTopServerTask.inc

#
# fab: could push and process at other location
#
if [ -n "$1" ] ; then
    BTOP_SERVER_DBPATH=$1
fi

Logs_add "PPTask" "record Raw Data"

# PROCESS RAW DATA
BTopServer_recordRawData

# PUSH NEW TASK
timestamp=$(date -u +"%Y%m%d%H%M00")
tz=$(BTopServer_getTimeZone)
start=$(php /var/www/html/BluePHP/Utils/TimeZoneConvert.php $timestamp UTC $tz)

# previous minute used to force the lastPushTask value to avoid big
# problems when there are to much tasks to do
prevMinute=$(Date_timestampsubminutes $start 1)
BTopServerTask_forceLastPushTask $prevMinute

start=$(Date_convertTimeStamp $start "true")

Logs_info PPTask "add new task ($start)"
    
$BTOP_SERVER_BCLTASK_CMD "$BTOP_SERVER_DBPATH" pushTasks "$start"

BTopServerTask_putTasksInPrev "$start"

# PROCESS TASK
Logs_info PPTask "process task"
    
$BTOP_SERVER_BCLTASK_CMD "$BTOP_SERVER_DBPATH" processTasks
    
BTopServerTask_clearTasksInPrev

# update the last processed date
BTopServer_updateProcessTime "$prevMinute"

BTopServerTask_unlockForProcessTask "$idprocess"

# POST PROCESS LATE COUNTER 
Logs_info PPTask "post process late counter"
$BTOP_SERVER_BCLPOSTPROCESS "$BTOP_SERVER_DBPATH" "$start"

Logs_info PPTask "end"
