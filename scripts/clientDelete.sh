#!/bin/bash
#
# Delete an existing client :
# delete the directories
#
# Parameters :
# $1 : clientId (STRING)

. /etc/BEV/scripts/BlueSystem/Logs.inc
. /etc/BEV/scripts/BlueSystem/Cron.inc
. /etc/BEV/scripts/BTopLocalServer/BTopServerTask.inc

function usage()
{
    echo "clientDelete.sh clientBluePortail"
    echo " clientBluePortail : the client BluePortail unique id"
}

if [ -z "$1" ] ; then
    usage
    Logs_add BluePortail::clientDelete "missing argument"
    exit -1
fi

CLIENTID=$1
CLIENTDATAROOT=/home/DATA/BluePortail/clients/$CLIENTID/

Logs_add BluePortail::clientDelete "info : Delete directory"

if [ ! -d $CLIENTDATAROOT ] ; then
    Logs_add BluePortail:clientDelete "error : missing client directory $CLIENTDATAROOT"
else
    ( rm -fr $CLIENTDATAROOT ) 2> /dev/null
fi

Logs_add BluePortail::clientDelete "info : Delete directory links"

if [ ! -L /var/www/BlueCountGUI/styles/$CLIENTID ] ; then
    Logs_add BluePortail:clientDelete "error : missing symbolic link /var/www/BlueCountGUI/styles/$CLIENTID"
else
    ( rm -f /var/www/BlueCountGUI/styles/$CLIENTID ) 2> /dev/null
fi

Logs_add BluePortail::clientDelete "info : remove pushAndProcessTask for this client"
tmpfile=/tmp/clienDelete$$
Cron_getCronFile ${BTOP_SERVER_TASK_PROCESS_USER} $tmpfile
Cron_removeCommandWithId $CLIENTID $tmpfile
Cron_setCronFile ${BTOP_SERVER_TASK_PROCESS_USER} $tmpfile
rm -f $tmpfile

Logs_add BluePortail::clientDelete "info : Done"
