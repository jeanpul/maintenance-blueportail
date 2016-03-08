#!/bin/bash
#
# Create a new client :
# 1 - create the directory
# 2 - copy the skel data in it
# 3 - add the link for the Web access
#
# Parameters :
# $1 : clientBluePortail (STRING)

. /etc/BEV/scripts/BlueSystem/Logs.inc
. /etc/BEV/scripts/BlueSystem/Cron.inc
. /etc/BEV/scripts/BTopLocalServer/BTopServerTask.inc

function usage()
{
    echo "clientCreate.sh clientBluePortail"
    echo " clientBluePortail : the client BluePortail unique id"
}

if [ -z "$1" ] ; then
    usage
    Logs_add BluePortail::clientCreate "missing argument"
    exit -1
fi

# global variables
CLIENTID=$1
BLUEPORTAILSKEL=/home/DATA/BluePortail/skel
CLIENTDATAROOT=/home/DATA/BluePortail/clients/$CLIENTID/


Logs_add BluePortail::clientCreate "info : Check installation"
if [ ! -d /var/www/BlueCountGUI ] ; then
    Logs_add BluePortail::clientCreate "fatal error : MISSING MODULE BlueCountGUI"
    exit -1
fi

if [ ! -d /var/www/BTopLocalServer ] ; then
    Logs_add BluePortail::clientCreate "fatal error : MISSING MODULE BTopLocalServer"
    exit -1
fi

Logs_add BluePortail::clientCreate "info : Check BluePortail client data directory"
if [ -d $CLIENTDATAROOT ] ; then
    Logs_add BluePortail::clientCreate "fatal error : CLIENT $CLIENTID BLUEPORTAIL DIRECTORY ALREADY EXIST"
    exit -1
fi

Logs_add BluePortail::clientCreate "info : Create directory and copy skel"
( mkdir -p $CLIENTDATAROOT && cp -fr $BLUEPORTAILSKEL/* $CLIENTDATAROOT/ ) 2> /dev/null
if [ $? -ne 0 ] ; then
    Logs_add BluePortail::clientCreate "fatal error : cannot create $CLIENTDATAROOT directory"
    exit -1
fi

Logs_add BluePortail::clientCreate "info : Copy configuration files from BTopLocalServer and BlueCountGUI"
( cp -f /var/www/BlueCountGUI/ConfigBluePortail.inc $CLIENTDATAROOT/BlueCountGUI/ && cp -f /var/www/BTopLocalServer/ConfigBluePortail.inc $CLIENTDATAROOT/BTopLocalServer/ ) 2> /dev/null
if [ $? -ne 0 ] ; then
    Logs_add BluePortail::clientCreate "fatal error : cannot copy BluePortail configuration files"
    exit -1
fi

Logs_add BluePortail::clientCreate "info : Create BlueCountServer databases"
( /etc/BEV/BlueCount/default/createAllTables.sh $CLIENTDATAROOT/BTopLocalServer/ ) 2> /dev/null
if [ $? -ne 0 ] ; then
    Logs_add BluePortail::clientCreate "fatal error : cannot create databases"
    exit -1
fi

Logs_add BluePortail::clientCreate "info : Create BlueCountGUI databases"
( cat /home/DATA/BlueCountGUI/createDatabase.sql | sqlite3 $CLIENTDATAROOT/BlueCountGUI/session.db ) 2> /dev/null
if [ $? -ne 0 ] ; then
    Logs_add BluePortail::clientCreate "fatal error : cannot create databases"
    exit -1
fi

Logs_add BluePortail::clientCreate "info : Change CLIENTBLUEPORTAIL in BluePortail configuration file"
sed -i "s%^define.*(.*CLIENTBLUEPORTAIL.*,.*%define('CLIENTBLUEPORTAIL',\"$CLIENTID\");%" $CLIENTDATAROOT/BluePortail/Config.inc 2> /dev/null
if [ $? -ne 0 ] ; then
    Logs_add BluePortail::clientCreate "fatal error : cannot change CLIENTBLUEPORTAIL in BluePortail configuration file"
    exit -1
fi

Logs_add BluePortail::clientCreate "info : Change BASEPATH of ConfigBluePortails"
( sed -i "s%^define.*(.*BASEPATH.*,.*%define('BASEPATH',\"$CLIENTDATAROOT\");%" $CLIENTDATAROOT/BTopLocalServer/ConfigBluePortail.inc && sed -i "s%^define.*(.*BASEPATH.*,.*%define('BASEPATH',\"$CLIENTDATAROOT\");%" $CLIENTDATAROOT/BlueCountGUI/ConfigBluePortail.inc ) 2> /dev/null
if [ $? -ne 0 ] ; then
    Logs_add BluePortail::clientCreate "fatal error : cannot change BASEPATH of ConfigBluePortails"
    exit -1
fi

Logs_add BluePortail::clientCreate "info : Links client data in BlueCountGUI"
(cd /var/www/BlueCountGUI/styles && ln -s $CLIENTDATAROOT/BlueCountGUI/www/styles $CLIENTID)

Logs_add BluePortail::clientCreate "info : Change Logo Path for BlueCountGUI"
echo "UPDATE Config SET value=\"$CLIENTDATAROOT/BlueCountGUI/www/styles/Logo.gif\" WHERE name=\"LOGO_ABSOLUTE_PATH\";" | sqlite3 $CLIENTDATAROOT/BlueCountGUI/session.db
if [ $? -ne 0 ] ; then
    Logs_add BluePortail::clientCreate "fatal error : cannot change LOGO_ABSOLUTE_PATH in $CLIENTDATAROOT/BlueCountGUI/session.db"
    exit -1
fi
echo "UPDATE Config SET value=\"$CLIENTID/Logo.gif\" WHERE name=\"LOGO_RELATIVE_PATH\";" | sqlite3 $CLIENTDATAROOT/BlueCountGUI/session.db
if [ $? -ne 0 ] ; then
    Logs_add BluePortail::clientCreate "fatal error : cannot change LOGO_RELATIVE_PATH in $CLIENTDATAROOT/BlueCountGUI/session.db"
    exit -1
fi

# LogoDefault will be the default Logo 
cp -f /var/www/BlueCountGUI/styles/LogoDefault.gif $CLIENTDATAROOT/BlueCountGUI/www/styles/Logo.gif

Logs_add BluePortail::clientCreate "info : Add the pushAndProcessTask for this client"
cmd="* * * * * ( ${BTOP_SERVER_TASK_PROCESS_CMD} $CLIENTDATAROOT/BTopLocalServer/ > /dev/null 2>&1 )"
Cron_addCommandWithId ${BTOP_SERVER_TASK_PROCESS_USER} $CLIENTID "$cmd"

Logs_add BluePortail::clientCreate "info : Done"
